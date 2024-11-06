#pragma once

#include <cstddef>

namespace svetit::db::utils {

template<std::size_t>
constexpr std::size_t GetTypeIndexByIdIndexImpl() {
	return -1;
}

template<std::size_t idIndex, std::size_t idx, std::size_t... idn>
constexpr std::size_t GetTypeIndexByIdIndexImpl() {
	if constexpr (idIndex == 0)
		return idx;
	else
		return GetTypeIndexByIdIndexImpl<idIndex - 1, idn...>();
}

template<std::size_t idIndex, template<std::size_t...> typename T, std::size_t... Is>
constexpr std::size_t GetTypeIndexByIdIndex(T<Is...>) {
	return GetTypeIndexByIdIndexImpl<idIndex, Is...>();
}

} // namespace svetit::db::utils
