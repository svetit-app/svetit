#pragma once

#include <boost/uuid/uuid.hpp>

namespace svetit::project::model {

struct SaveTimer {
	int id;
	boost::uuids::uuid projectId;
	int intervalMsec;
};

} // namespace svetit::project::model