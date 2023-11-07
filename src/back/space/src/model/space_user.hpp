#pragma once

#include <string>
#include <chrono>
#include <userver/utils/boost_uuid4.hpp>
#include <boost/uuid/uuid.hpp>

namespace svetit::space::model {

struct SpaceUser {
	boost::uuids::uuid spaceId;
	boost::uuids::uuid userId;
	bool isOwner;
	std::chrono::system_clock::time_point joinedAt;
	std::string role;
};

} // namespace svetit::space::model