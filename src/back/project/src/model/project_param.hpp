#pragma once

#include <boost/uuid/uuid.hpp>
#include <cstdint>

#include <shared/db/db_table_helpers.hpp>

namespace svetit::project::model {

struct ProjectParam {
	using Ids = db::Ids<0, 1, 2>;
	using FilterIds = db::Ids<0, 1>;
	using NoInsertIds = db::Ids<>;

	boost::uuids::uuid spaceId;
	boost::uuids::uuid projectId;
	int64_t paramId;
};

} // namespace svetit::project::model