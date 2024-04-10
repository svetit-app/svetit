#pragma once

#include <string>

namespace svetit::project::model {

struct CcModeType {
	int id;
	int ccTypeId;
	std::string key;
	std::string name;
	bool isDeleted = false;
};

} // namespace svetit::project::model