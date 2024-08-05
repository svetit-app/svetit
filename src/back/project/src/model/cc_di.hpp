#pragma once

#include <shared/db/db_table_helpers.hpp>

#include <boost/uuid/uuid.hpp>

namespace svetit::project::model {

struct CcDi {
	using Ids = db::Ids<0, 1, 2>;
	using FilterIds = db::Ids<0, 2>;
	using NoInsertIds = db::Ids<>;

	int64_t ccId;
	int64_t diId;
	boost::uuids::uuid spaceId;
};

} // namespace svetit::project::model
