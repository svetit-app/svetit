#pragma once

#include "../../utils/enableiftype.hpp"

#include <cstddef>
#include <tuple>

namespace svetit::db::utils {

template<typename T, class Enable = void>
struct IdsTuple {
	using type = std::index_sequence<0>;
	static constexpr std::tuple<std::size_t> Get() { return {0}; }
};

template<typename T>
struct IdsTuple<T, typename svetit::utils::enable_if_type<typename T::Ids>::type> {
	using type = T::Ids;
	static constexpr auto Get() { return T::Ids::Get(); }
};

template<typename T, class Enable = void>
struct FilterIdsTuple {
	using type = std::index_sequence<>;
	static constexpr std::tuple<> Get() { return {}; }
};

template<typename T>
struct FilterIdsTuple<T, typename svetit::utils::enable_if_type<typename T::FilterIds>::type> {
	using type = T::FilterIds;
	static constexpr auto Get() { return T::FilterIds::Get(); }
};

template<typename T, class Enable = void>
struct NoInsertIdsTuple : IdsTuple<T, Enable> {};

template<typename T>
struct NoInsertIdsTuple<T, typename svetit::utils::enable_if_type<typename T::NoInsertIds>::type> {
	using type = T::NoInsertIds;
	static constexpr auto Get() { return T::NoInsertIds::Get(); }
};

template<typename T, class Enable = void>
struct NoUpdateIdsTuple {
	using type = decltype(std::tuple_cat(std::make_tuple(typename IdsTuple<T>::type{}), std::make_tuple(typename NoInsertIdsTuple<T>::type{})));
	static constexpr auto Get() { return std::tuple_cat(IdsTuple<T>::Get(), NoInsertIdsTuple<T>::Get()); }
};

template<typename T>
struct NoUpdateIdsTuple<T, typename svetit::utils::enable_if_type<typename T::NoUpdateIds>::type> {
	using type = T::NoUpdateIds;
	static constexpr auto Get() { return T::NoUpdateIds::Get(); }
};

} // namespace svetit::db::utils
