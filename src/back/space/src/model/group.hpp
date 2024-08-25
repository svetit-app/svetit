#pragma once

#include <shared/db/db_table_helpers.hpp>

#include <boost/uuid/uuid.hpp>
#include <string>

namespace svetit::space::model {

struct Group {
	using Ids = db::Ids<0, 1>;
	using FilterIds = db::Ids<1>;

	int id;
	boost::uuids::uuid spaceId;
	std::string name;
	std::string description;
};

} // namespace svetit::space::model
