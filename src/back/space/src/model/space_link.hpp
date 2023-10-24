#pragma once

#include <string>
#include <chrono>

namespace svetit::space::model {

struct SpaceLink {
	std::string id;
	std::string spaceId;
	std::string creatorId;
	std::string name;
	std::chrono::system_clock::time_point createdAt;
	std::chrono::system_clock::time_point expiredAt;
};

} // namespace svetit::space::model