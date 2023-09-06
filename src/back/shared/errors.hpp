#pragma once

#include <stdexcept>

namespace svetit::errors {

struct NotFound final : public std::runtime_error {
	NotFound() : std::runtime_error{"resource not found"} {}
};

struct ForceBlock final : public std::runtime_error {
	ForceBlock() : std::runtime_error{"force block"} {}
};

} // namespace svetit::errors
