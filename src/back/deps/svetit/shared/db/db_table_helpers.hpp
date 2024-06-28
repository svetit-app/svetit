#pragma once

#include <tuple>

namespace svetit::db {

template<std::size_t... ids>
struct Ids {
	static constexpr std::tuple<decltype(ids)...> Get() { return {ids...}; }
	static constexpr std::size_t Size() noexcept { return sizeof...(ids); }
};

} // namespace svetit::db
