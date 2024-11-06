#pragma once

#include <svetit/db/db_table_helpers.hpp>

#include <boost/uuid/uuid.hpp>
#include <string>

namespace svetit::node::model {

struct Node {
	using Ids = db::Ids<0, 1>;
	using FilterIds = db::Ids<1>;

	boost::uuids::uuid id;
	boost::uuids::uuid spaceId;
	std::string name;
	std::string description;
	double latitude;
	double longitude;
	int64_t createdAt;
};

} // namespace svetit::node::model
