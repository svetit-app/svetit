#pragma once

#include "param_value_type.hpp"
#include <cstdint>
#include <boost/uuid/uuid.hpp>

namespace svetit::project::model {

struct ParamType {
	int64_t id;
	boost::uuids::uuid spaceId;
	std::optional<int64_t> parentId;
	std::string key;
	std::string name;
	std::string description;
	ParamValueType::Type valueType;
};

} // namespace svetit::project::model