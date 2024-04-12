#pragma once

#include <string>

namespace svetit::project::model {

struct CcModeType {
	int id;
	int ccTypeId;
	std::string key;
	std::string name;
};

} // namespace svetit::project::model