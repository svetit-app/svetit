#pragma once

#include <string>
#include <cstdint>

namespace svetit::project::model {

struct ControlCircuit {
	int64_t id;
	int64_t typeId;
	int64_t sectionId;
	std::string name;
};

} // namespace svetit::project::model