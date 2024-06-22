#pragma once

#include "sync_direction.hpp"
#include <shared/db/db_table_helpers.hpp>

#include <boost/uuid/uuid.hpp>

namespace svetit::project::model {

struct Project {
	using Ids = db::Ids<0, 1>;

	boost::uuids::uuid id;
	boost::uuids::uuid spaceId;
	std::string key;
	std::string name;
	std::string description;
	std::chrono::system_clock::time_point changedAt;
	SyncDirection::Type sync;
};

} // namespace svetit::project::model
