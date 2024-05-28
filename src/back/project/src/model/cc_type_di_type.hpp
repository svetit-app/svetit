#pragma once

#include <cstdint>
#include <boost/uuid/uuid.hpp>

namespace svetit::project::model {

struct CcTypeDiType {
	boost::uuids::uuid spaceId;
	int64_t ccTypeId;
	int64_t diTypeId;
};

} // namespace svetit::project::model