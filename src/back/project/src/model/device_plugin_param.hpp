#pragma once

#include <cstdint>
#include <boost/uuid/uuid.hpp>

#include <shared/db/db_table_helpers.hpp>

namespace svetit::project::model {

struct DevicePluginParam {
	using Ids = db::Ids<0, 1, 2>;
	using FilterIds = db::Ids<0, 1>;
	using NoInsertIds = db::Ids<>;

	boost::uuids::uuid spaceId;
	int64_t deviceId;
	int64_t paramId;
};

} // namespace svetit::project::model