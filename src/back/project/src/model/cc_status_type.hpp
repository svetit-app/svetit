#pragma once

#include <string>
#include <cstdint>

namespace svetit::project::model {

struct CcStatusType {
	int64_t id;
	int64_t ccTypeId;
	int64_t categoryId;
	std::string key;
	std::string text;
	bool inform;
};

} // namespace svetit::project::model