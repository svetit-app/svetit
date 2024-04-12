#pragma once

#include "param_value_type.hpp"

namespace svetit::project::model {

struct ParamType {
	int id;
	std::optional<int> parentId;
	std::string key;
	std::string name;
	std::string description;
	ParamValueType::Type valueType;
};

} // namespace svetit::project::model