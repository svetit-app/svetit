#pragma once

#include <shared/db/db_table_helpers.hpp>

#include <boost/uuid/uuid.hpp>
#include <string>
#include <chrono>

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
	std::chrono::system_clock::time_point createdAt;
};

} // namespace svetit::node::model
