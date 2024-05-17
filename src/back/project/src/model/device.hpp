#pragma once

#include <string>
#include <boost/uuid/uuid.hpp>
#include <cstdint>

namespace svetit::project::model {

struct Device {
	int64_t id;
	boost::uuids::uuid projectId;
	int64_t pluginId;
	std::string name;
	int checkIntervalMsec;
};

} // namespace svetit::project::model