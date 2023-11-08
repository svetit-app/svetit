#pragma once

#include <stdexcept>

namespace svetit::errors {

struct BadRequest final : public std::runtime_error {
	using std::runtime_error::runtime_error;
};

} // namespace svetit::errors