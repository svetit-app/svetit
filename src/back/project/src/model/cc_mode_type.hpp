#pragma once

#include <string>
#include <cstdint>
#include <boost/uuid/uuid.hpp>

namespace svetit::project::model {

struct CcModeType {
	int64_t id;
	boost::uuids::uuid spaceId;
	boost::uuids::uuid projectId;
	int64_t ccTypeId;
	std::string key;
	std::string name;
};

} // namespace svetit::project::model