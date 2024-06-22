#pragma once

#include "boost/pfr/core.hpp"
#include "db_base.hpp"
#include "../errors.hpp"
#include "../strings/camel2snake.hpp"

#include <boost/hana/fwd/contains.hpp>
#include <boost/pfr/core_name.hpp>
#include <boost/type_index.hpp>
#include <boost/tokenizer.hpp>

#include <tuple>
#include <type_traits>
#include <utility>

namespace svetit::db {

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

template<class T, class R = void>  
struct enable_if_type { typedef R type; };

template<typename T, class Enable = void>
struct IdsTuple {
	static constexpr std::tuple<std::size_t> Get() { return {0}; }
};

template<typename T>
struct IdsTuple<T, typename enable_if_type<typename T::Ids>::type> {
	static constexpr auto Get() { return T::Ids::Get(); }
};

template <typename T, typename V>
bool ChecTupleContains(const T&, const V&) {
	return false;
};

template <typename T, typename V, std::size_t i, std::size_t... Is>
bool ChecTupleContains(const T& t, const V& v) {
	if (std::get<i>(t) == v)
		return true;
	return ChecTupleContains<T, V, Is...>(t, v);
};

template <typename T, typename V, std::size_t... Is>
bool TupleContains(const T& t, const V& v, std::index_sequence<Is...>) {
	return ChecTupleContains<T, V, Is...>(t, v);
}

template <typename T, typename V, std::size_t TSize = std::tuple_size_v<T>>
bool TupleContains(const T& t, const V& v) {
	return TupleContains(t, v, std::make_index_sequence<TSize>{});
}

template<typename FieldsT, typename ArgsT, typename T>
constexpr bool CheckIdType(const T&) {
	return true;
}

constexpr auto GetTypeByIndex() {
}

template<typename FieldsT, typename ArgsT, typename T, std::size_t i, std::size_t... Is>
constexpr bool CheckIdType(const T& t) {
	using ArgT = std::tuple_element_t<i, ArgsT>;
	using IdT = std::tuple_element_t<std::get<i>(t), FieldsT>;
	static_assert(
		std::is_convertible_v<ArgT, IdT>,
#if __cpp_static_assert >= 202306L
		std::format("[svetit] Expected Id type {}, got {}", typeid(IdT).name(), typeid(ArgT).name()));
#else
		"[svetit] Ids args types mismatch for table function");
#endif
	return CheckIdType<FieldsT, ArgsT, T, Is...>(t);
}

template<typename FieldsT, typename ArgsT, typename T, std::size_t... Is>
constexpr bool CheckIdsTypes(const T& t, std::index_sequence<Is...>) {
	return CheckIdType<FieldsT, ArgsT, T, Is...>(t);
}

template<typename FieldsT, typename ArgsT, typename T, std::size_t TSize = std::tuple_size_v<T>>
constexpr bool CheckIdsTypes(const T& t) {
	return CheckIdsTypes<FieldsT, ArgsT>(t, std::make_index_sequence<TSize>{});
}

template<typename, typename...> struct IsIds;

template<typename T, typename... Args>
struct IsIds {
	static constexpr bool CheckSize() {
		static_assert(
			sizeof...(Args) == std::tuple_size<decltype(IdsTuple<T>::Get())>::value,
#if __cpp_static_assert >= 202306L
			std::format("[svetit] Expected Ids args {}, got {}", std::tuple_size<decltype(IdsTuple<T>::Get())>::value, sizeof...(Args)));
#else
			"[svetit] Ids args count mismatch for table function");
#endif

		return true;
	}
	static constexpr bool CheckTypes() {
		auto idsIndexes = IdsTuple<T>::Get();
		using ArgsT = std::tuple<Args...>;
		using FieldsT = decltype(boost::pfr::structure_to_tuple(T{}));
		return CheckIdsTypes<FieldsT, ArgsT>(idsIndexes);
	}
	static constexpr std::size_t value = CheckSize() && CheckTypes();
};

template<typename T>
struct Table {
	static std::string TableName();
	static std::string TableFieldsString();

	void Update(const T& item);

template<typename... Args>
typename std::enable_if<IsIds<T, Args...>::value>::type Delete2(Args&&... args) {
	static const auto deleteSql = []() -> storages::postgres::Query {
		auto names = boost::pfr::names_as_array<T>();
		auto idsIndexes = IdsTuple<T>::Get();
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
		auto idsIndexes = IdsTuple<T>::Get();
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

} // namespace svetit::db
