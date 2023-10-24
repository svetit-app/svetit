#pragma once

#include <string>
#include <chrono>

namespace svetit::space::model {

struct SpaceUser {
	std::string spaceId;
	std::string userId;
	bool isOwner;
	std::chrono::system_clock::time_point joinedAt;
	std::string role;
};

} // namespace svetit::space::model