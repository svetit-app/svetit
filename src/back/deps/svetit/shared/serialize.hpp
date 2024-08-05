#pragma once

#include <shared/utils/enableiftype.hpp>

#include <type_traits>
#include <userver/formats/json/value_builder.hpp>
#include <userver/formats/parse/common_containers.hpp>
#include <userver/formats/serialize/common_containers.hpp>
#include <userver/utest/using_namespace_userver.hpp>

#include <boost/pfr/core.hpp>
#include <boost/pfr/core_name.hpp>

namespace svetit {

template<class T, class Enable = void>
struct HasIdsType : std::false_type {};

template<typename T>
struct HasIdsType<T, typename svetit::utils::enable_if_type<typename T::Ids>::type> : std::true_type {};

template<class T, class Enable = void>
struct HasFilterIdsType : std::false_type {};

template<typename T>
struct HasFilterIdsType<T, typename svetit::utils::enable_if_type<typename T::FilterIds>::type> : std::true_type {};

template<class T, class Enable = void>
struct HasNoInsertIdsType : std::false_type {};

template<typename T>
struct HasNoInsertIdsType<T, typename svetit::utils::enable_if_type<typename T::NoInsertIds>::type> : std::true_type {};

template<class T, class Enable = void>
struct HasNoUpdateIdsType : std::false_type {};

template<typename T>
struct HasNoUpdateIdsType<T, typename svetit::utils::enable_if_type<typename T::NoUpdateIds>::type> : std::true_type {};

template<class T, class Enable = void>
struct HasAutoParseType : std::false_type {};

template<typename T>
struct HasAutoParseType<T, typename svetit::utils::enable_if_type<typename T::AutoParse>::type> : std::true_type {};

template<typename T>
struct IsParsable {
	static constexpr bool value = []() {
		if constexpr (HasAutoParseType<T>::value)
			return T::AutoParse::value;
		else
			return HasIdsType<T>::value
				|| HasFilterIdsType<T>::value
				|| HasNoInsertIdsType<T>::value
				|| HasNoUpdateIdsType<T>::value;
	}();
};

template<typename T>
std::enable_if_t<IsParsable<T>::value, formats::json::Value> Serialize(
	const T& item,
	formats::serialize::To<formats::json::Value>)
{
	formats::json::ValueBuilder res;
	const auto names = boost::pfr::names_as_array<T>();

	boost::pfr::for_each_field(item, [&res, &names](const auto& field, auto i) {
		const std::string name{names.at(i)};
		res[name] = field;
	});

	return res.ExtractValue();
}

template<typename T>
std::enable_if_t<IsParsable<T>::value, T> Parse(
	const formats::json::Value& json,
	formats::parse::To<T>)
{
	T item;
	const auto names = boost::pfr::names_as_array<T>();

	boost::pfr::for_each_field(item, [&json, &names](auto& field, auto i) {
		const std::string name{names.at(i)};
		field = json[name].As<std::decay_t<decltype(field)>>(formats::json::Value::DefaultConstructed{});
	});

	return item;
}

} // namespace svetit
