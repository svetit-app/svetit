#pragma once

#include <string>
#include <boost/uuid/uuid.hpp>

namespace svetit::project::model {

struct Device {
	int id;
	boost::uuids::uuid projectId;
	int pluginId;
	std::string name;
	int checkIntervalMsec;
	bool isDeleted = false;
};

} // namespace svetit::project::model