#pragma once

#include <string>
#include <boost/uuid/uuid.hpp>
#include <cstdint>

namespace svetit::project::model {

struct Code {
	int64_t id;
	boost::uuids::uuid spaceId;
	boost::uuids::uuid projectId;
	boost::uuids::uuid repositoryId;
	std::string commitHash;
};

} // namespace svetit::project::model