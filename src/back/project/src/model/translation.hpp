#pragma once

#include <string>
#include <boost/uuid/uuid.hpp>
#include <cstdint>

namespace svetit::project::model {

struct Translation {
	int64_t id;
	boost::uuids::uuid spaceId;
	boost::uuids::uuid projectId;
	std::string lang;
	std::string key;
	std::string value;
};

} // namespace svetit::project::model