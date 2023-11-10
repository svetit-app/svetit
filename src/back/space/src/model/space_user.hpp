#pragma once

#include "role.hpp"

#include <string>
#include <chrono>
#include <userver/utils/boost_uuid4.hpp>
#include <boost/uuid/uuid.hpp>

namespace svetit::space::model {

struct SpaceUser {
	boost::uuids::uuid spaceId;
	std::string userId;
	bool isOwner;
	int64_t joinedAt;
	Role::Type role;
};

} // namespace svetit::space::model