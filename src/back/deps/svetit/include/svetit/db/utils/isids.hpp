#pragma once

#include <svetit/db/utils/idstuple.hpp>
#include <svetit/db/utils/gettypeindexbyidindex.hpp>

#include <tuple>
#include <boost/pfr/core.hpp>
#include <boost/pfr/core_name.hpp>

namespace svetit::db::utils {

template<typename ArgT, typename IdT, std::size_t idTypeIndex>
constexpr void CheckIdTypeHelper() {
	static_assert(
		std::is_convertible_v<ArgT, IdT>,
#if __cpp_static_assert >= 202306L
		std::format("[svetit] Expected Id type {}, got {}", typeid(IdT).name(), typeid(ArgT).name()));
#else
		"[svetit] Id arg type mismatch for table function");
#endif
}

template<typename T, typename FieldsT, typename ArgsT, typename IdsT>
constexpr bool CheckIdType() {
	return true;
}

template<typename T, typename FieldsT, typename ArgsT, typename IdsT, std::size_t i, std::size_t... Is>
constexpr bool CheckIdType() {
	using ArgT = std::tuple_element_t<i, ArgsT>;
	constexpr std::size_t idTypeIndex = GetTypeIndexByIdIndex<i>(IdsT{});
	using IdT = std::tuple_element_t<idTypeIndex, FieldsT>;
	CheckIdTypeHelper<ArgT, IdT, idTypeIndex>();
	return CheckIdType<T, FieldsT, ArgsT, IdsT, Is...>();
}

template<typename T, typename FieldsT, typename ArgsT, typename IdsT, std::size_t... Is>
constexpr bool CheckIdsTypes(std::index_sequence<Is...>) {
	return CheckIdType<T, FieldsT, ArgsT, IdsT, Is...>();
}

template<typename T, typename FieldsT, typename ArgsT, typename IdsT, typename IdsTupleT, std::size_t TSize = std::tuple_size_v<IdsTupleT>>
constexpr bool CheckIdsTypes() {
	return CheckIdsTypes<T, FieldsT, ArgsT, IdsT>(std::make_index_sequence<TSize>{});
}

template<typename T, typename, typename, typename...> struct IsIdsImpl;

template<typename T, typename IdsT, typename IdsTupleT, typename... Args>
struct IsIdsImpl {
	static constexpr bool CheckSize() {
		static_assert(
			sizeof...(Args) == std::tuple_size<IdsTupleT>::value,
#if __cpp_static_assert >= 202306L
			std::format("[svetit] Expected Ids args {}, got {}", std::tuple_size<IdsTupleT>::value, sizeof...(Args)));
#else
			"[svetit] Ids args count mismatch for table function");
#endif

		return true;
	}
	static constexpr bool CheckTypes() {
		using ArgsT = std::tuple<Args...>;
		using FieldsT = decltype(boost::pfr::structure_to_tuple(T{}));
		return CheckIdsTypes<T, FieldsT, ArgsT, IdsT, IdsTupleT>();
	}
	static constexpr std::size_t value = CheckSize() && CheckTypes();
};

template<typename T, typename... Args>
using IsIds = IsIdsImpl<T, typename IdsTuple<T>::type, decltype(IdsTuple<T>::Get()), Args...>;

template<typename T, typename... Args>
using IsFilterIds = IsIdsImpl<T, typename FilterIdsTuple<T>::type, decltype(FilterIdsTuple<T>::Get()), Args...>;

} // namespace svetit::db::utils
