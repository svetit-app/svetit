#pragma once

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
	std::string role;
};

} // namespace svetit::space::model