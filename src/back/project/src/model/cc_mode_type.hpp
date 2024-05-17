#pragma once

#include <string>
#include <cstdint>

namespace svetit::project::model {

struct CcModeType {
	int64_t id;
	int64_t ccTypeId;
	std::string key;
	std::string name;
};

} // namespace svetit::project::model