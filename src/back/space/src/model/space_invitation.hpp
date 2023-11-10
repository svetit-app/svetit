#pragma once

#include <string>
#include <userver/utils/boost_uuid4.hpp>
#include <boost/uuid/uuid.hpp>

#include "role.hpp"

namespace svetit::space::model {

struct SpaceInvitation {
	int id;
	boost::uuids::uuid spaceId;
	std::string creatorId;
	std::string userId;
	Role::Type role;
	int64_t createdAt;
};

} // namespace svetit::space::model