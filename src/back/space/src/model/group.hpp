#pragma once

#include <boost/uuid/uuid.hpp>
#include <string>
#include <chrono>

namespace svetit::space::model {

struct Group {
	int id;
	std::string name;
	std::string description;
	boost::uuids::uuid spaceId;
};

} // namespace svetit::space::model
