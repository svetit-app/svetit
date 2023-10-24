#pragma once

#include <string>
#include <chrono>

namespace svetit::space::model {

struct SpaceInvitation {
	int id;
	std::string spaceId;
	std::string creatorId;
	std::string userId;
	std::string role;
	std::chrono::system_clock::time_point createdAt;
};

} // namespace svetit::space::model