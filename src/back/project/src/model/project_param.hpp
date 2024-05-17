#pragma once

#include <boost/uuid/uuid.hpp>
#include <cstdint>

namespace svetit::project::model {

struct ProjectParam {
	boost::uuids::uuid projectId;
	int64_t paramId;
};

} // namespace svetit::project::model