#pragma once

#include <stdexcept>

namespace svetit::errors {

struct NotFound final : public std::runtime_error {
	NotFound() : std::runtime_error{"resource not found"} {}
};

struct Unauthorized : public std::runtime_error {
	Unauthorized() : std::runtime_error{"Access denied"} {};
};

struct Conflict : public std::runtime_error {
	using std::runtime_error::runtime_error;
};

struct BadRequest : public std::runtime_error {
	using std::runtime_error::runtime_error;
};

struct NotModified : public std::runtime_error {
	NotModified() : std::runtime_error{"Not modified"} {};
};

struct Forbidden403 : public std::runtime_error {
	Forbidden403() : std::runtime_error{"Access forbidden"} {};
};

} // namespace svetit::errors
