#pragma once

#include <string>
#include <cstdint>
#include <boost/uuid/uuid.hpp>

#include <svetit/db/db_table_helpers.hpp>

namespace svetit::project::model {

struct DeviceItem {
	using Ids = db::Ids<0, 1>;
	using FilterIds = db::Ids<1, 2>;

	int64_t id;
	boost::uuids::uuid spaceId;
	int64_t deviceId;
	int64_t typeId;
	std::string name;
};

} // namespace svetit::project::model
