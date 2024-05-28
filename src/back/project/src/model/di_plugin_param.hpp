#pragma once

#include <cstdint>
#include <boost/uuid/uuid.hpp>

namespace svetit::project::model {

struct DiPluginParam {
	boost::uuids::uuid spaceId;
	int64_t diTypeId;
	int64_t paramId;
};

} // namespace svetit::project::model