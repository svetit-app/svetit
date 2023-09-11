#pragma once

#include <stdexcept>

namespace svetit::errors {

struct ForceBlock final : public std::runtime_error {
	ForceBlock(const std::string& msg) : std::runtime_error{msg} {}
};

} // namespace svetit::errors