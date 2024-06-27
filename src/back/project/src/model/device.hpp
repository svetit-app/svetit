#pragma once

#include <string>
#include <boost/uuid/uuid.hpp>
#include <cstdint>

#include <shared/db/db_table_helpers.hpp>

namespace svetit::project::model {

struct Device {
	using Ids = db::Ids<0, 1>;
	using FilterIds = db::Ids<1, 2>;

	int64_t id;
	boost::uuids::uuid spaceId;
	boost::uuids::uuid projectId;
	int64_t pluginId;
	std::string name;
	int checkIntervalMsec;
};

} // namespace svetit::project::model