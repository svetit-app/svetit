#pragma once

#include <tuple>

template<std::size_t I = 0, typename ...From, typename ...To>
typename std::enable_if<(I >= sizeof...(From) || I >= sizeof...(To))>::type
moveTuple(std::tuple<From...>&&, std::tuple<To...>&) {}

template<std::size_t I = 0, typename ...From, typename ...To>
typename std::enable_if<(I < sizeof...(From) && I < sizeof...(To))>::type
moveTuple(std::tuple<From...>&& from, std::tuple<To...>& to)
{
	std::get<I>(to) = std::move(std::get<I>(from));
	moveTuple<I + 1>(std::move(from), to);
}
