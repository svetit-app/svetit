#pragma once

#include <cstdint>
#include <boost/uuid/uuid.hpp>

#include <svetit/db/db_table_helpers.hpp>

namespace svetit::project::model {

struct CcTypeDiType {
	using Ids = db::Ids<0, 1, 2>;
	using FilterIds = db::Ids<0, 1>;
	using NoInsertIds = db::Ids<>;

	boost::uuids::uuid spaceId;
	int64_t ccTypeId;
	int64_t diTypeId;
};

} // namespace svetit::project::model
