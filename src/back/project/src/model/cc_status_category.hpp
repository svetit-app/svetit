#pragma once

#include <string>
#include <boost/uuid/uuid.hpp>

namespace svetit::project::model {

struct CcStatusCategory {
	int id;
	boost::uuids::uuid projectId;
	std::string key;
	std::string name;
	std::string color;
};

} // namespace svetit::project::model