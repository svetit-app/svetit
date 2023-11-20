#pragma once

#include <stdexcept>

namespace svetit::errors {

struct NotFound final : public std::runtime_error {
	NotFound() : std::runtime_error{"resource not found"} {}
};

struct Unauthorized : public std::runtime_error {
	using std::runtime_error::runtime_error;
};

struct Conflict : public std::runtime_error {
	using std::runtime_error::runtime_error;
};

struct BadRequest : public std::runtime_error {
	using std::runtime_error::runtime_error;
};

} // namespace svetit::errors
