#pragma once

#include <string>

namespace svetit::project::model {

struct CcModeType {
	int id;
	int ccTypeId;
	std::string key;
	std::string name;
	bool isDeleted;
};

} // namespace svetit::project::model