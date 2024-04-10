#pragma once

#include "param_value_type.hpp"

namespace svetit::project::model {

struct ParamType {
	int id;
	int parentId;
	std::string key;
	std::string name;
	std::string description;
	ParamValueType::Type valueType;
	bool isDeleted = false;
};

} // namespace svetit::project::model