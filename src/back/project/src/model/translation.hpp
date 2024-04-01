#pragma once

#include <string>
#include <boost/uuid/uuid.hpp>

namespace svetit::project::model {

struct Translation {
	int id;
	boost::uuids::uuid projectId;
	std::string lang;
	std::string key;
	std::string value;
};

} // namespace svetit::project::model