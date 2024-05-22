#pragma once

#include <cstdint>
#include <boost/uuid/uuid.hpp>

namespace svetit::project::model {

struct DevicePluginParam {
	boost::uuids::uuid spaceId;
	int64_t deviceId;
	int64_t paramId;
};

} // namespace svetit::project::model