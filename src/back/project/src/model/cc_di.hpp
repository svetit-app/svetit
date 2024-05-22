#pragma once

#include <cstdint>
#include <boost/uuid/uuid.hpp>

namespace svetit::project::model {

struct CcDi {
	boost::uuids::uuid spaceId;
	int64_t ccId;
	int64_t diId;
};

} // namespace svetit::project::model