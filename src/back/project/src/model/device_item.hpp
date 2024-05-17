#pragma once

#include <string>
#include <cstdint>

namespace svetit::project::model {

struct DeviceItem {
	int64_t id;
	int64_t deviceId;
	int64_t typeId;
	std::string name;
};

} // namespace svetit::project::model