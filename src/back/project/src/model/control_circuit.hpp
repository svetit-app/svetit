#pragma once

#include <string>

namespace svetit::project::model {

struct ControlCircuit {
	int id;
	int typeId;
	int sectionId;
	std::string name;
};

} // namespace svetit::project::model