#pragma once

#include <cstdint>
#include <boost/uuid/uuid.hpp>

#include <shared/db/db_table_helpers.hpp>

namespace svetit::project::model {

struct CcParam {
	using Ids = db::Ids<0, 1, 2>;
	using FilterIds = db::Ids<0, 1>;

	boost::uuids::uuid spaceId;
	int64_t ccId;
	int64_t paramId;
};

} // namespace svetit::project::model