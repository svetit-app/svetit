#pragma once

#include "db_base.hpp"
#include "db_table_utils.hpp"
#include "../errors.hpp"
#include "../strings/camel2snake.hpp"
#include "../paging.hpp"
#include "userver/storages/postgres/io/row_types.hpp"

#include <tuple>
#include <userver/storages/postgres/cluster_types.hpp>

#include <boost/type_index.hpp>
#include <boost/tokenizer.hpp>
//#include <boost/hana/for_each.hpp>
#include <boost/hana.hpp>

#include <vector>

namespace svetit::db {

// Использование шаблона db::Table
// Для описания состовных идентификаторов необходимо перечислить
// порядковые номера полей идентификаторов в типе Ids внутри модели.
// В случае отсутствия типа Ids, идентификатором будет считатся 
// первое поле в структуре.
// Этот список также будет использован для формирования аргументов
// для функции Get и Delete.
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
// UPDATE project.project SET key=$3, ... WHERE id=$1 AND spaceId=$2
// DELETE FROM project.project WHERE id=$1 AND spaceId=$2
// SELECT id, ..., COUNT(*) OVER() FROM project.project WHERE space_id = $3 OFFSET $1 LIMIT $2

template<typename T>
struct Table {
	static std::string TableName();
	static std::vector<std::string> TableFields();
	static std::string TableFieldsString();

	Table(std::shared_ptr<Base> dbPtr)
		: _db{std::move(dbPtr)} {}

	template<typename... Args>
	typename std::enable_if<utils::IsIds<T, Args...>::value, T>::type Get(Args&&... args);

	auto Create(const T& item);
	void Update(const T& item);

	template<typename... Args>
	typename std::enable_if<utils::IsIds<T, Args...>::value>::type Delete(Args&&... args);

	template<typename... Args>
	typename std::enable_if<utils::IsFilterIds<T, Args...>::value, PagingResult<T>>::type GetList2(int start, int limit, Args&&... args);

protected:
	std::shared_ptr<Base> _db;
};

template<typename T>
inline std::string Table<T>::TableName()
{
	auto res = []() {
		const auto fullName = boost::typeindex::type_id<T>().pretty_name();
		boost::char_separator<char> sep("::");
		boost::tokenizer tokens(fullName, sep);

		std::string schema, name;
		for(auto&& it : tokens) {
			if (it == "model")
				continue;
			if (!name.empty())
				schema = std::move(name);
			name = std::move(it);
		}

		name = Camel2Snake(name);
		if (schema.empty())
			return name;

		schema = Camel2Snake(schema);
		schema += '.';
		schema += std::move(name);
		return schema;
	}();
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

			if (!fields.empty())
				fields += ", ";
			fields += fmt::format("{}=${}", name, nameIndex);
		}

		return {
			fmt::format("SELECT {} FROM {} WHERE {}", fields, TableName(), cond),
			storages::postgres::Query::Name{"get_" + TableName()},
		};
	}();

	auto res = _db->Execute(storages::postgres::ClusterHostType::kMaster, selectSql, std::forward<Args>(args)...);
	if (res.IsEmpty())
		throw errors::NotFound404{};

	return res.template AsSingleRow<T>(storages::postgres::kRowTag);
}

template<typename T>
auto Table<T>::Create(const T& item)
{
	static const auto createSql = []() -> storages::postgres::Query {
		auto names = TableFields();
		auto idsIndexes = utils::IdsTuple<T>::Get();
		std::string fields, values, idRet;
		std::size_t nameIndex = 0;
		for (auto&& name : names) {
			if (utils::TupleContains(idsIndexes, nameIndex++)) {
				if (idRet.empty())
					idRet = fmt::format("{},${}", name, nameIndex);

				if (name == "id")
					continue;
			}

			if (!fields.empty()) {
				fields += ", ";
				values += ", ";
			}
			fields += name;
			values += fmt::format("${}", nameIndex);
		}

		if (idRet.empty() && !names.empty())
			idRet = fmt::format("{},${}", names.front(), 1);

		return {
			fmt::format("INSERT INTO {} ({}) VALUES({}) RETURNING {}", TableName(), fields, values, idRet),
			storages::postgres::Query::Name{"create_" + TableName()},
		};
	}();


	auto res = std::apply([&](const auto&... args) {
		return _db->Execute(storages::postgres::ClusterHostType::kMaster, createSql, args...);
	}, boost::pfr::structure_tie(item));

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
		std::string cond, fields;
		std::size_t nameIndex = 0;
		for (auto&& name : names) {
			if (utils::TupleContains(idsIndexes, nameIndex++)) {
				if (!cond.empty())
					cond += " AND ";
				cond += fmt::format("{}=${}", name, nameIndex);
				continue;
			}

			if (!fields.empty())
				fields += ", ";
			fields += fmt::format("{}=${}", name, nameIndex);
		}

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
template<typename... Args>
inline typename std::enable_if<utils::IsFilterIds<T, Args...>::value, PagingResult<T>>::type Table<T>::GetList2(int start, int limit, Args&&... args) {
	static const auto listSql = []() -> storages::postgres::Query {
		auto names = TableFields();
		auto idsIndexes = utils::FilterIdsTuple<T>::Get();
		std::string cond;
		std::size_t nameIndex = 2;

		std::tuple<std::size_t, std::size_t> idsIndexes2{1, 2};
		boost::hana::for_each(idsIndexes2, [&names, &cond, &nameIndex](std::size_t i) {
			if (i < 0 || i >= names.size())
				return;

			if (!cond.empty())
				cond += " AND ";
			cond += fmt::format("{}=${}", names.at(i), ++nameIndex);
		});

		if (!cond.empty())
			cond.insert(0, "WHERE ");

		return {
			fmt::format("SELECT {}, COUNT(*) OVER() FROM {} {} OFFSET $1 LIMIT $2", TableFieldsString(), TableName(), cond),
			storages::postgres::Query::Name{"list_" + TableName()},
		};
	}();

	auto res = _db->Execute(storages::postgres::ClusterHostType::kSlave, listSql, start, limit, std::forward<Args>(args)...);

	PagingResult<T> data;
	data = res.AsContainer<decltype(data)::RawContainer>(storages::postgres::kRowTag);
	return data;
}

} // namespace svetit::db
