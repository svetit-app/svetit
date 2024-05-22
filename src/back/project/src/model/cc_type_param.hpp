#pragma once

#include <cstdint>
#include <boost/uuid/uuid.hpp>

namespace svetit::project::model {

struct CcTypeParam {
	boost::uuids::uuid spaceId;
	int64_t ccTypeId;
	int64_t paramId;
};

} // namespace svetit::project::model