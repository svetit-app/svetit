#pragma once

#include "param_value_type.hpp"
#include <cstdint>
#include <boost/uuid/uuid.hpp>

#include <shared/db/db_table_helpers.hpp>

namespace svetit::project::model {

struct ParamType {
	using Ids = db::Ids<0, 1>;
	using FilterIds = db::Ids<1, 2>;
	using NoInsertIds = db::Ids<0>;

	int64_t id;
	boost::uuids::uuid spaceId;
	boost::uuids::uuid projectId;
	std::optional<int64_t> parentId;
	std::string key;
	std::string name;
	std::string description;
	ParamValueType::Type valueType;
};

} // namespace svetit::project::model