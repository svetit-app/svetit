#pragma once

#include <boost/uuid/uuid.hpp>

#include "sync_direction.hpp"

namespace svetit::project::model {

struct Project {
	boost::uuids::uuid id;
	boost::uuids::uuid spaceId;
	std::string key;
	std::string name;
	std::string description;
	std::chrono::system_clock::time_point changedAt;
	SyncDirection::Type sync;
};

} // namespace svetit::project::model