#pragma once

#include <string>
#include <cstdint>
#include <boost/uuid/uuid.hpp>

namespace svetit::project::model {

struct DeviceItem {
	int64_t id;
	boost::uuids::uuid spaceId;
	int64_t deviceId;
	int64_t typeId;
	std::string name;
};

} // namespace svetit::project::model
