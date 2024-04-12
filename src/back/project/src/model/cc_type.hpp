#pragma once

#include <string>
#include <boost/uuid/uuid.hpp>

namespace svetit::project::model {

struct CcType {
	int id;
	boost::uuids::uuid projectId;
	std::string key;
	std::string name;
	std::string description;
};

} // namespace svetit::project::model