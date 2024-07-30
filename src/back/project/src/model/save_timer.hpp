#pragma once

#include <boost/uuid/uuid.hpp>
#include <cstdint>

#include <shared/db/db_table_helpers.hpp>

namespace svetit::project::model {

struct SaveTimer {
	using Ids = db::Ids<0, 1>;
	using FilterIds = db::Ids<1, 2>;

	int64_t id;
	boost::uuids::uuid spaceId;
	boost::uuids::uuid projectId;
	int intervalMsec;
};

} // namespace svetit::project::model