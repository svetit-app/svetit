#pragma once

#include <string>
#include <boost/uuid/uuid.hpp>

namespace svetit::project::model {

struct Plugin {
	int id;
	boost::uuids::uuid projectId;
	std::string name;
	std::string description;
	std::string key;
	bool isDeleted;
};

} // namespace svetit::project::model