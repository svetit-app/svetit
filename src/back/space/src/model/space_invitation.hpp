#pragma once

#include <boost/uuid/uuid.hpp>
#include <chrono>
#include <optional>

#include "role.hpp"

namespace svetit::space::model {

struct SpaceInvitation {
	int id;
	boost::uuids::uuid spaceId;
	std::string creatorId;
	std::string userId;
	std::optional<int> roleId;
	std::chrono::system_clock::time_point createdAt;
};

} // namespace svetit::space::model