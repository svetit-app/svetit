#pragma once

#include <string>
#include <cstdint>
#include <boost/uuid/uuid.hpp>

namespace svetit::project::model {

struct ControlCircuit {
	int64_t id;
	boost::uuids::uuid spaceId;
	int64_t typeId;
	int64_t sectionId;
	std::string name;
};

} // namespace svetit::project::model