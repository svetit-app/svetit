#pragma once

#include <boost/uuid/uuid.hpp>
#include <cstdint>

namespace svetit::project::model {

struct SaveTimer {
	int64_t id;
	boost::uuids::uuid projectId;
	int intervalMsec;
};

} // namespace svetit::project::model