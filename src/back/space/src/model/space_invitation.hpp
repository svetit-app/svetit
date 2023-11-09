#pragma once

#include <string>
#include <userver/utils/boost_uuid4.hpp>
#include <boost/uuid/uuid.hpp>

namespace svetit::space::model {

struct SpaceInvitation {
	int id;
	boost::uuids::uuid spaceId;
	std::string creatorId;
	std::string userId;
	std::string role;
	int64_t createdAt;
};

} // namespace svetit::space::model