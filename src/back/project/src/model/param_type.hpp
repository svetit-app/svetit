#pragma once

#include "param_value_type.hpp"
#include <cstdint>

namespace svetit::project::model {

struct ParamType {
	int64_t id;
	std::optional<int64_t> parentId;
	std::string key;
	std::string name;
	std::string description;
	ParamValueType::Type valueType;
};

} // namespace svetit::project::model