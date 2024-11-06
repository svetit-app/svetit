#pragma once

#include <svetit/db/db_table_helpers.hpp>

#include <string>
#include <cstdint>
#include <boost/uuid/uuid.hpp>

namespace svetit::project::model {

struct CcModeType {
	using Ids = db::Ids<0, 1>;
	using FilterIds = db::Ids<1, 3>;

	int64_t id;
	boost::uuids::uuid spaceId;
	boost::uuids::uuid projectId;
	int64_t ccTypeId;
	std::string key;
	std::string name;
};

} // namespace svetit::project::model
