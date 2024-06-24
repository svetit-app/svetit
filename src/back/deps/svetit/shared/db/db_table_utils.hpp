#pragma once

#include <tuple>
#include <boost/pfr/core.hpp>
#include <boost/pfr/core_name.hpp>

namespace svetit::db::utils {

template<class T, class R = void>
struct enable_if_type { typedef R type; };

template<typename T, class Enable = void>
struct IdsTuple {
	using type = std::index_sequence<0>;
	static constexpr std::tuple<std::size_t> Get() { return {0}; }
};

template<typename T>
struct IdsTuple<T, typename enable_if_type<typename T::Ids>::type> {
	using type = T::Ids;
	static constexpr auto Get() { return T::Ids::Get(); }
};

template<typename T, class Enable = void>
struct FilterIdsTuple {
	using type = std::index_sequence<>;
	static constexpr std::tuple<> Get() { return {}; }
};

template<typename T>
struct FilterIdsTuple<T, typename enable_if_type<typename T::FilterIds>::type> {
	using type = T::Ids;
	static constexpr auto Get() { return T::FilterIds::Get(); }
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

template<std::size_t>
constexpr std::size_t GetTypeIndexByIdIndexImpl() {
	return -1;
}

template<std::size_t idIndex, std::size_t idx, std::size_t... idn>
constexpr std::size_t GetTypeIndexByIdIndexImpl() {
	if constexpr (idIndex == 0)
		return idx;
	return GetTypeIndexByIdIndexImpl<idIndex - 1, idn...>();
}

template<std::size_t idIndex, template<std::size_t...> typename T, std::size_t... Is>
constexpr std::size_t GetTypeIndexByIdIndex(T<Is...>) {
	return GetTypeIndexByIdIndexImpl<idIndex, Is...>();
}

template<typename T, typename FieldsT, typename ArgsT, typename IdsT>
constexpr bool CheckIdType() {
	return true;
}

template<typename T, typename FieldsT, typename ArgsT, typename IdsT, std::size_t i, std::size_t... Is>
constexpr bool CheckIdType() {
	using ArgT = std::tuple_element_t<i, ArgsT>;
	constexpr std::size_t idTypeIndex = GetTypeIndexByIdIndex<i>(typename IdsTuple<T>::type{});
	using IdT = std::tuple_element_t<idTypeIndex, FieldsT>;
	static_assert(
		std::is_convertible_v<ArgT, IdT>,
#if __cpp_static_assert >= 202306L
		std::format("[svetit] Expected Id type {}, got {}", typeid(IdT).name(), typeid(ArgT).name()));
#else
		"[svetit] Id arg type mismatch for table function");
#endif
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
