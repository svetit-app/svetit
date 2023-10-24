#pragma once

#include <string>
#include <chrono>

namespace svetit::space::model {

struct Space {
	std::string id;
	std::string name;
	std::string key;
	bool requestsAllowed;
	std::chrono::system_clock::time_point createdAt;
};

} // namespace svetit::space::model