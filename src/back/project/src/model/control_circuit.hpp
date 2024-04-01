#pragma once

#include <string>
#include <boost/uuid/uuid.hpp>

namespace svetit::project::model {

struct ControlCircuit {
	int id;
	int typeId;
	int sectionId;
	std::string name;
	bool isDeleted;
};

} // namespace svetit::project::model