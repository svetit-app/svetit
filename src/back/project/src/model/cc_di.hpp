#pragma once

#include <cstdint>
#include <boost/uuid/uuid.hpp>

namespace svetit::project::model {

struct CcDi {
	int64_t ccId;
	int64_t diId;
	boost::uuids::uuid spaceId;
};

} // namespace svetit::project::model
