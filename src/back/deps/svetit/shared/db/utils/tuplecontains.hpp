#pragma once

#include <tuple>

namespace svetit::db::utils {

template <typename T, typename V>
bool CheckTupleContains(const T&, const V&) {
	return false;
};

template <typename T, typename V, std::size_t i, std::size_t... Is>
bool CheckTupleContains(const T& t, const V& v) {
	if (std::get<i>(t) == v)
		return true;
	return CheckTupleContains<T, V, Is...>(t, v);
};

template <typename T, typename V, std::size_t... Is>
bool TupleContains(const T& t, const V& v, std::index_sequence<Is...>) {
	return CheckTupleContains<T, V, Is...>(t, v);
}

template <typename T, typename V, std::size_t TSize = std::tuple_size_v<T>>
bool TupleContains(const T& t, const V& v) {
	return TupleContains(t, v, std::make_index_sequence<TSize>{});
}

} // namespace svetit::db::utils
