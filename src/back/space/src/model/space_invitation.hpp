#pragma once

#include <string>
#include <chrono>
#include <userver/utils/boost_uuid4.hpp>
#include <boost/uuid/uuid.hpp>

namespace svetit::space::model {

struct SpaceInvitation {
	int id;
	boost::uuids::uuid spaceId;
	boost::uuids::uuid creatorId;
	boost::uuids::uuid userId;
	std::string role;
	std::chrono::system_clock::time_point createdAt;
};

} // namespace svetit::space::model