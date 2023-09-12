#pragma once

#include <stdexcept>

namespace svetit::errors {

struct SecurityRisk final : public std::runtime_error {
	SecurityRisk(const std::string& msg) : std::runtime_error{msg} {}
};

} // namespace svetit::errors