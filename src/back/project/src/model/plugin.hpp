#pragma once

#include <string>
#include <boost/uuid/uuid.hpp>
#include <cstdint>

namespace svetit::project::model {

struct Plugin {
	int64_t id;
	boost::uuids::uuid projectId;
	std::string name;
	std::string description;
	std::string key;
};

} // namespace svetit::project::model