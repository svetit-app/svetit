#pragma once

#include <string>
#include <boost/uuid/uuid.hpp>

namespace svetit::project::model {

struct Code {
	int id;
	boost::uuids::uuid projectId;
	boost::uuids::uuid repositoryId;
	std::string commitHash;
};

} // namespace svetit::project::model