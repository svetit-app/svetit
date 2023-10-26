#pragma once

#include <string>
#include <chrono>
#include <boost/uuid/uuid.hpp>

namespace svetit::space::model {

struct SpaceLink {
	boost::uuids::uuid id;
	boost::uuids::uuid spaceId;
	boost::uuids::uuid creatorId;
	std::string name;
	std::chrono::system_clock::time_point createdAt;
	std::chrono::system_clock::time_point expiredAt;
};

} // namespace svetit::space::model