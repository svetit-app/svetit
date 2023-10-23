#pragma once

#include <string>
#include <chrono>

namespace svetit::space {

struct Space {
	std::string id;
	std::string name;
	std::string key;
	bool requestsAllowed;
	std::chrono::system_clock::time_point createdAt;
};

struct SpaceInvitation {
	int id;
	std::string spaceId;
	std::string creatorId;
	std::string userId;
	std::string role;
	std::chrono::system_clock::time_point createdAt;
};

struct SpaceLink {
	std::string id;
	std::string spaceId;
	std::string creatorId;
	std::string name;
	std::chrono::system_clock::time_point createdAt;
	std::chrono::system_clock::time_point expiredAt;
};

struct SpaceUser {
	std::string spaceId;
	std::string userId;
	bool isOwner;
	std::chrono::system_clock::time_point Date;
	std::string role;
};

struct SpaceFields {
	std::string spaceName;
};

} // namespace svetit::space