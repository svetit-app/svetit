#pragma once

#include <string>
#include <boost/uuid/uuid.hpp>

namespace svetit::space::model {

struct SpaceUser {
	boost::uuids::uuid spaceId;
	std::string userId;
	bool isOwner;
	int64_t joinedAt;
	int roleId;
};

} // namespace svetit::space::model
