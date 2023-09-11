#pragma once

#include <stdexcept>

namespace svetit::errors {

struct NotFound final : public std::runtime_error {
	NotFound() : std::runtime_error{"resource not found"} {}
};

} // namespace svetit::errors
