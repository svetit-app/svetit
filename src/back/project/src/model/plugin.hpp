#pragma once

#include <string>
#include <boost/uuid/uuid.hpp>
#include <cstdint>

#include <svetit/db/db_table_helpers.hpp>

namespace svetit::project::model {

struct Plugin {
	using Ids = db::Ids<0, 1>;
	using FilterIds = db::Ids<1, 2>;

	int64_t id;
	boost::uuids::uuid spaceId;
	boost::uuids::uuid projectId;
	std::string name;
	std::string description;
	std::string key;
};

} // namespace svetit::project::model