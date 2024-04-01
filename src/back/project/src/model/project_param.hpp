#pragma once

#include <boost/uuid/uuid.hpp>

namespace svetit::project::model {

struct ProjectParam {
	boost::uuids::uuid projectId;
	int paramId;
	bool isDeleted;
};

} // namespace svetit::project::model