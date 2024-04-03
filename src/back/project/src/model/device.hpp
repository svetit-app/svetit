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
	bool isDeleted;
};

} // namespace svetit::project::model