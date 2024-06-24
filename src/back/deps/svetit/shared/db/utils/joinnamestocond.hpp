#pragma once

#include <tuple>
#include <vector>
#include <fmt/format.h>

namespace svetit::db::utils {

template <std::size_t i = 0, typename... Ts>
std::string joinNamesToCond(const std::vector<std::string>& names, std::size_t nameIndex, std::tuple<Ts...> t)
{
	if constexpr(i == sizeof...(Ts))
		return {};
	else {
		const auto x = std::get<i>(t);
		std::string first = fmt::format("{}=${}", names.at(x), nameIndex)
			, second = joinNamesToCond<i + 1>(names, nameIndex + 1, t);
		if (!second.empty()) {
			first += " AND ";
			first += std::move(second);
		}
		return first;
	}
}

} // namespace svetit::db::utils
