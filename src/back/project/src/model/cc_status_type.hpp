#pragma once

#include <string>
#include <cstdint>
#include <boost/uuid/uuid.hpp>

#include <shared/db/db_table_helpers.hpp>

namespace svetit::project::model {

struct CcStatusType {
	using Ids = db::Ids<0, 1>;
	using FilterIds = db::Ids<1, 2>;

	int64_t id;
	boost::uuids::uuid spaceId;
	int64_t ccTypeId;
	int64_t categoryId;
	std::string key;
	std::string text;
	bool inform;
};

} // namespace svetit::project::model