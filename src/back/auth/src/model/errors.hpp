#pragma once

#include <stdexcept>

namespace svetit::errors {

struct ForceBlock final : public std::runtime_error {
	ForceBlock() : std::runtime_error{"force block"} {}
};

} // namespace svetit::errors