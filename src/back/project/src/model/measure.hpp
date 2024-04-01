#pragma once

#include <string>
#include <boost/uuid/uuid.hpp>

namespace svetit::project::model {

struct Measure {
	int id;
	boost::uuids::uuid projectId;
	std::string name;
	bool isDeleted;
};

} // namespace svetit::project::model