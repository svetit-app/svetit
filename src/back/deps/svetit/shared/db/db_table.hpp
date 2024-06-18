#pragma once

#include "db_base.hpp"
#include "../errors.hpp"
#include "../strings/camel2snake.hpp"

#include <boost/pfr/core_name.hpp>
#include <boost/type_index.hpp>
#include <boost/tokenizer.hpp>

namespace svetit::db {

template<typename T>
struct Table {
	static std::string TableName();
	static std::string TableFieldsString();

	void Update(const T& item);

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
		std::string fields = "project_id = $3, parent_id = $4, key = $5, name = $6, description = $7, value_type = $8";
		std::string cond = "id = $1 AND space_id = $2";
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

} // namespace svetit::db
