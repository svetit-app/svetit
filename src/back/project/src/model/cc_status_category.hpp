#pragma once

#include <string>
#include <boost/uuid/uuid.hpp>
#include <cstdint>

#include <shared/db/db_table_helpers.hpp>

namespace svetit::project::model {

struct CcStatusCategory {
	using Ids = db::Ids<0, 1>;
	using FilterIds = db::Ids<1, 2>;
	using NoInsertIds = db::Ids<0>;

	int64_t id;
	boost::uuids::uuid spaceId;
	boost::uuids::uuid projectId;
	std::string key;
	std::string name;
	std::string color;
};

} // namespace svetit::project::model