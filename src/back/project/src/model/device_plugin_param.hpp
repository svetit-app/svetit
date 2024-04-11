#pragma once

namespace svetit::project::model {

struct DevicePluginParam {
	int deviceId;
	int paramId;
	bool isDeleted = false;
};

} // namespace svetit::project::model