#pragma once

#include <boost/uuid/uuid.hpp>
#include <string>
#include <optional>

namespace svetit::space::model {

struct SpaceInvitation {
	int id;
	boost::uuids::uuid spaceId;
	std::string creatorId;
	std::string userId;
	std::optional<int> roleId;
	int64_t createdAt;
};

} // namespace svetit::space::model
