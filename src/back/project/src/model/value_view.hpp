#pragma once

#include <string>
#include <cstdint>
#include <boost/uuid/uuid.hpp>

#include <shared/db/db_table_helpers.hpp>

namespace svetit::project::model {

struct ValueView {
	using Ids = db::Ids<0, 1>;
	using FilterIds = db::Ids<1, 2>;
	using NoInsertIds = db::Ids<0>;

	int64_t id;
	boost::uuids::uuid spaceId;
	int64_t diTypeId;
	std::string value;
	std::string view;
};

} // namespace svetit::project::model