#pragma once

#include "db_base.hpp"
#include "db_table_utils.hpp"
#include "../errors.hpp"
#include "../strings/camel2snake.hpp"

#include <boost/type_index.hpp>
#include <boost/tokenizer.hpp>

#include <type_traits>

namespace svetit::db {

// Использование шаблона db::Table
// Для описания состовных идентификаторов необходимо перечислить
// порядковые номера полей идентификаторов в типе Ids внутри модели.
// В случае отсутствия типа Ids, идентификатором будет считатся 
// первое поле в структуре.
// Этот список также будет использован для формирования аргументов
// для функции Delete.
//
// Например для структуры
// namespace svetit::project::model {
// struct Project {
// 	using Ids = db::Ids<0, 1>;
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

template<typename T>
struct Table {
	static std::string TableName();
	static std::string TableFieldsString();

	void Update(const T& item);

	template<typename... Args>
	typename std::enable_if<utils::IsIds<T, Args...>::value>::type Delete(Args&&... args);

protected:
	std::shared_ptr<Base> _db;
};

template<typename T>
inline std::string Table<T>::TableName()
{
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
}

template<typename T>
inline std::string Table<T>::TableFieldsString()
{
	auto names = boost::pfr::names_as_array<T>();
	std::size_t size = 0;
	for (auto&& it: names)
		size += it.size() + 1;
	if (size == 0)
		return std::string{};

	std::string res;
	res.reserve(size * 1.2f);
	res.resize(size);

	for (auto&& it: names)
	{
		res += Camel2Snake(it);
		res += ',';
	}

	res.resize(res.size() - 1);
	return res;
}

template<typename T>
void Table<T>::Update(const T& item) {
	static const auto updateSql = []() -> storages::postgres::Query {
		auto names = boost::pfr::names_as_array<T>();
		auto idsIndexes = utils::IdsTuple<T>::Get();
		std::string cond, fields;
		std::size_t nameIndex = 0;
		for (auto&& name : names) {
			if (TupleContains(idsIndexes, nameIndex++)) {
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
		auto names = boost::pfr::names_as_array<T>();
		auto idsIndexes = utils::IdsTuple<T>::Get();
		std::string cond;
		std::size_t nameIndex = 0;
		for (auto&& name : names) {
			if (TupleContains(idsIndexes, nameIndex++)) {
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

} // namespace svetit::db
