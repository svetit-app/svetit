#pragma once

#include "role.hpp"
#include <boost/uuid/uuid.hpp>

namespace svetit::space::model {

struct SpaceUser {
	boost::uuids::uuid spaceId;
	std::string userId;
	bool isOwner;
	std::chrono::system_clock::time_point joinedAt;
	Role::Type role;
};

} // namespace svetit::space::model