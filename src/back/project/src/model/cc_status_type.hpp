#pragma once

#include <string>
#include <cstdint>
#include <boost/uuid/uuid.hpp>

namespace svetit::project::model {

struct CcStatusType {
	int64_t id;
	boost::uuids::uuid spaceId;
	int64_t ccTypeId;
	int64_t categoryId;
	std::string key;
	std::string text;
	bool inform;
};

} // namespace svetit::project::model