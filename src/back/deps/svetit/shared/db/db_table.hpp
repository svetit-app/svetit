#pragma once

#include "db_base.hpp"
#include "utils/isids.hpp"
#include "utils/joinnamestocond.hpp"
#include "utils/tuplecontains.hpp"
#include "utils/tablename.hpp"
#include "../errors.hpp"
#include "../strings/camel2snake.hpp"
#include "../paging.hpp"

#include <userver/storages/postgres/cluster_types.hpp>
#include <userver/storages/postgres/io/row_types.hpp>

#include <boost/type_index.hpp>
#include <boost/tokenizer.hpp>

namespace svetit::db {

// Использование шаблона db::Table
// Для описания состовных идентификаторов необходимо перечислить
// порядковые номера полей идентификаторов в типе Ids внутри модели.
// В случае отсутствия типа Ids, идентификатором будет считатся
// первое поле в структуре.
// Этот список также будет использован для формирования аргументов
// для функции Get, Update и Delete.
// Для указания столбцов в качестве фильтров при получении списка,
// можно добавить тип FilterIds внутри модели.
//
// Например для структуры
// namespace svetit::project::model {
// struct Project {
// 	using Ids = db::Ids<0, 1>;
// 	using FilterIds = db::Ids<1>;
//
// 	boost::uuids::uuid id;
// 	boost::uuids::uuid spaceId;
// 	std::string key;
// 	// остальные поля...
// };
// } // ns
//
// Будут сгенерированы такие запросы:
// INSERT INTO project.project (space_id, key, ...) VALUES($2, $3, ...) RETURNING id, $1
// SELECT id, ... FROM project.project WHERE id=$1 AND space_id=$2
// UPDATE project.project SET key=$3, ... WHERE id=$1 AND space_id=$2
// DELETE FROM project.project WHERE id=$1 AND space_id=$2
// SELECT id, ..., COUNT(*) OVER() FROM project.project WHERE space_id = $3 OFFSET $1 LIMIT $2

template<typename T>
struct Table {
	static std::string TableName();
	static std::vector<std::string> TableFields();
	static std::string TableFieldsString();

	Table(std::shared_ptr<Base> dbPtr)
		: _db{std::move(dbPtr)} {}

	auto Create(const T& item);
	void Update(const T& item);

	template<typename... Args>
	typename std::enable_if<utils::IsIds<T, Args...>::value, T>::type Get(Args&&... args);

	template<typename A>
	auto CallGet(const A& args);

	template<typename... Args>
	typename std::enable_if<utils::IsIds<T, Args...>::value>::type Delete(Args&&... args);

	template<typename A>
	void CallDelete(const A& args);

	template<typename... Args>
	typename std::enable_if<utils::IsFilterIds<T, Args...>::value, PagingResult<T>>::type GetList(int start, int limit, Args&&... args);

	template<typename A>
	auto CallGetList(const A& args);

protected:
	std::shared_ptr<Base> _db;

private:
	template<typename A, std::size_t... Is>
	auto callGet(const A& args, std::index_sequence<Is...>);

	template<typename A, std::size_t... Is>
	void callDelete(const A& args, std::index_sequence<Is...>);

	template<typename A, std::size_t... Is>
	auto callGetList(const A& args, std::index_sequence<Is...>);
};

template<typename T>
inline std::string Table<T>::TableName()
{
	static constexpr auto nameArr = utils::TableName<T>();
	static std::string res = nameArr.data();
	return res;
}

template<typename T>
inline std::vector<std::string> Table<T>::TableFields()
{
	static auto res = []() {
		auto names = boost::pfr::names_as_array<T>();
		std::vector<std::string> res;
		for (auto&& it: names)
			res.push_back(Camel2Snake(it));
		return res;
	}();
	return res;
}

template<typename T>
inline std::string Table<T>::TableFieldsString()
{
	static auto res = []() {
		auto names = TableFields();
		std::size_t size = names.size();
		for (auto&& it: names)
			size += it.size();
		if (size == 0)
			return std::string{};

		std::string res;
		res.reserve(size * 1.2f);

		for (auto&& it: names)
		{
			res += std::move(it);
			res += ',';
		}

		res.resize(res.size() - 1);
		return res;
	}();
	return res;
}

template<typename T>
auto Table<T>::Create(const T& item)
{
	static const auto createSql = []() -> storages::postgres::Query {
		auto names = TableFields();
		auto idsIndexes = utils::IdsTuple<T>::Get();
		auto skipFieldsIndexes = utils::NoInsertIdsTuple<T>::Get();
		std::string fields, values, idsRet, fieldsSkip;
		std::size_t nameIndex = 0;
		for (auto&& name : names) {
			if (utils::TupleContains(idsIndexes, nameIndex)) {
				if (!idsRet.empty())
					idsRet += ", ";
				idsRet += name;
			}

			if (utils::TupleContains(skipFieldsIndexes, nameIndex)) {
				if (!fieldsSkip.empty())
					fieldsSkip += ", ";
				fieldsSkip += fmt::format("${}", ++nameIndex);
				continue;
			}

			if (!fields.empty()) {
				fields += ", ";
				values += ", ";
			}
			fields += name;
			values += fmt::format("${}", ++nameIndex);
		}

		if (!idsRet.empty() && !fieldsSkip.empty()) {
			idsRet += ", ";
			idsRet += fieldsSkip;
		}

		if (!idsRet.empty())
			idsRet.insert(0, "RETURNING ");

		return {
			fmt::format("INSERT INTO {} ({}) VALUES({}) {}", TableName(), fields, values, idsRet),
			storages::postgres::Query::Name{"create_" + TableName()},
		};
	}();

	auto res = std::apply([&](const auto&... args) {
		return _db->Execute(storages::postgres::ClusterHostType::kMaster, createSql, args...);
	}, boost::pfr::structure_tie(item));

	// TODO: возвращать tuple со всеми Id
	constexpr std::size_t idTypeIndex = utils::GetTypeIndexByIdIndex<0>(typename utils::IdsTuple<T>::type{});
	using FieldsT = decltype(boost::pfr::structure_to_tuple(T{}));
	using IdT = std::tuple_element_t<idTypeIndex, FieldsT>;
	return res.Front()[0].template As<IdT>();
}

template<typename T>
void Table<T>::Update(const T& item) {
	static const auto updateSql = []() -> storages::postgres::Query {
		auto names = TableFields();
		auto idsIndexes = utils::IdsTuple<T>::Get();
		auto skipFieldsIndexes = utils::NoUpdateIdsTuple<T>::Get();
		std::string cond, fields, fieldsSkip;
		std::size_t nameIndex = 0;
		bool isIdField;
		for (auto&& name : names) {
			isIdField = false;
			if (utils::TupleContains(idsIndexes, nameIndex)) {
				if (!cond.empty())
					cond += " AND ";
				cond += fmt::format("{}=${}", name, nameIndex + 1);
				isIdField = true;
			}

			if (utils::TupleContains(skipFieldsIndexes, nameIndex)) {
				if (!isIdField) {
					if (!fieldsSkip.empty())
						fieldsSkip += ", ";
					fieldsSkip += fmt::format("${}", nameIndex + 1);
				}
				++nameIndex;
				continue;
			}

			if (!fields.empty())
				fields += ", ";
			fields += fmt::format("{}=${}", name, ++nameIndex);
		}

		if (!fieldsSkip.empty())
			cond += " RETURNING " + fieldsSkip;

		return {
			fmt::format("UPDATE {} SET {} WHERE {}", TableName(), fields, cond),
			storages::postgres::Query::Name{"update_" + TableName()},
		};
	}();

	auto res = std::apply([&](const auto&... args) {
		return _db->Execute(updateSql, args...);
	}, boost::pfr::structure_tie(item));
	if (!res.RowsAffected())
		throw errors::NotFound404();
}

template<typename T>
template<typename... Args>
typename std::enable_if<utils::IsIds<T, Args...>::value, T>::type Table<T>::Get(Args&&... args)
{
	static const auto selectSql = []() -> storages::postgres::Query {
		auto names = TableFields();
		auto idsIndexes = utils::IdsTuple<T>::Get();
		std::string cond, fields;
		std::size_t nameIndex = 0;
		for (auto&& name : names) {
			if (utils::TupleContains(idsIndexes, nameIndex++)) {
				if (!cond.empty())
					cond += " AND ";
				cond += fmt::format("{}=${}", name, nameIndex);
			}
		}

		return {
			fmt::format("SELECT {} FROM {} WHERE {}", TableFieldsString(), TableName(), cond),
			storages::postgres::Query::Name{"get_" + TableName()},
		};
	}();

	auto res = _db->Execute(storages::postgres::ClusterHostType::kMaster, selectSql, std::forward<Args>(args)...);
	if (res.IsEmpty())
		throw errors::NotFound404{};

	return res.template AsSingleRow<T>(storages::postgres::kRowTag);
}

template<typename T>
template<typename A>
inline auto Table<T>::CallGet(const A& args) {
	return callGet(args, std::make_index_sequence<std::tuple_size_v<A>>());
}

template<typename T>
template<typename... Args>
inline typename std::enable_if<utils::IsIds<T, Args...>::value>::type Table<T>::Delete(Args&&... args) {
	static const auto deleteSql = []() -> storages::postgres::Query {
		auto names = TableFields();
		auto idsIndexes = utils::IdsTuple<T>::Get();
		std::string cond;
		std::size_t nameIndex = 0;
		for (auto&& name : names) {
			if (utils::TupleContains(idsIndexes, nameIndex++)) {
				if (!cond.empty())
					cond += " AND ";
				cond += fmt::format("{}=${}", name, nameIndex);
				continue;
			}
		}

		return {
			fmt::format("DELETE FROM {} WHERE {}", TableName(), cond),
			storages::postgres::Query::Name{"delete_" + TableName()},
		};
	}();

	_db->Execute(storages::postgres::ClusterHostType::kMaster, deleteSql, std::forward<Args>(args)...);
}

template<typename T>
template<typename A>
inline void Table<T>::CallDelete(const A& args) {
	callDelete(args, std::make_index_sequence<std::tuple_size_v<A>>());
}

template<typename T>
template<typename... Args>
inline typename std::enable_if<utils::IsFilterIds<T, Args...>::value, PagingResult<T>>::type Table<T>::GetList(int start, int limit, Args&&... args) {
	static const auto listSql = []() -> storages::postgres::Query {
		const auto names = TableFields();
		const auto idsIndexes = utils::FilterIdsTuple<T>::Get();
		auto cond = utils::joinNamesToCond(names, 3, idsIndexes);
		if (!cond.empty())
			cond.insert(0, "WHERE ");

		return {
			fmt::format("SELECT {}, COUNT(*) OVER() FROM {} {} OFFSET $1 LIMIT $2", TableFieldsString(), TableName(), cond),
			storages::postgres::Query::Name{"list_" + TableName()},
		};
	}();

	auto res = _db->Execute(storages::postgres::ClusterHostType::kSlave, listSql, start, limit, std::forward<Args>(args)...);

	PagingResult<T> data;
	data = res.template AsContainer<typename decltype(data)::RawContainer>(storages::postgres::kRowTag);
	return data;
}

template<typename T>
template<typename A>
inline auto Table<T>::CallGetList(const A& args) {
	return callGetList(args, std::make_index_sequence<std::tuple_size_v<A>>());
}

template<typename T>
template<typename A, std::size_t... Is>
inline auto Table<T>::callGet(const A& args, std::index_sequence<Is...>) {
	return Get(std::get<Is>(args)...);
}

template<typename T>
template<typename A, std::size_t... Is>
inline void Table<T>::callDelete(const A& args, std::index_sequence<Is...>) {
	Delete(std::get<Is>(args)...);
}

template<typename T>
template<typename A, std::size_t... Is>
inline auto Table<T>::callGetList(const A& args, std::index_sequence<Is...>) {
	return GetList(std::get<Is>(args)...);
}

} // namespace svetit::db
