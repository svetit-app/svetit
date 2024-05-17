#pragma once

#include <string>
#include <boost/uuid/uuid.hpp>
#include <cstdint>

namespace svetit::project::model {

struct Section {
	int64_t id;
	boost::uuids::uuid projectId;
	std::string name;
};

} // namespace svetit::project::model