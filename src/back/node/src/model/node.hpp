#pragma once

#include <boost/uuid/uuid.hpp>
#include <string>
#include <chrono>

namespace svetit::node::model {

struct Node {
	boost::uuids::uuid id;
	boost::uuids::uuid spaceId;
	std::string name;
	std::string description;
	double latitude;
	double longitude;
	std::chrono::system_clock::time_point createdAt;
};

} // namespace svetit::project::model
