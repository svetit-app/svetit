#pragma once

#include <string>
#include <cstdint>
#include <boost/uuid/uuid.hpp>

namespace svetit::project::model {

struct DeviceItem {
	int64_t id;
	int64_t deviceId;
	int64_t typeId;
	std::string name;
	boost::uuids::uuid spaceId;
};

} // namespace svetit::project::model
