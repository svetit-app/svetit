#pragma once

#include <string>

namespace svetit::project::model {

struct CcStatusType {
	int id;
	int ccTypeId;
	int categoryId;
	std::string key;
	std::string text;
	bool inform;
};

} // namespace svetit::project::model