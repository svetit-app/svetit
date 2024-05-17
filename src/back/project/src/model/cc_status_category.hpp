#pragma once

#include <string>
#include <boost/uuid/uuid.hpp>
#include <cstdint>

namespace svetit::project::model {

struct CcStatusCategory {
	int64_t id;
	boost::uuids::uuid projectId;
	std::string key;
	std::string name;
	std::string color;
};

} // namespace svetit::project::model