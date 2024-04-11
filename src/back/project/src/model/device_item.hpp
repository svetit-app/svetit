#pragma once

#include <string>

namespace svetit::project::model {

struct DeviceItem {
	int id;
	int deviceId;
	int typeId;
	std::string name;
	bool isDeleted = false;
};

} // namespace svetit::project::model