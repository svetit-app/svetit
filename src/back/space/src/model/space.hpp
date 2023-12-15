#pragma once

#include <string>
#include <chrono>
#include <boost/uuid/uuid.hpp>

namespace svetit::space::model {

struct Space {
	boost::uuids::uuid id;
	std::string name;
	std::string key;
	bool requestsAllowed;
	std::chrono::system_clock::time_point createdAt;
};

} // namespace svetit::space::model