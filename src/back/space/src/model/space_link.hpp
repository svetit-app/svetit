#pragma once

#include <string>
#include <boost/uuid/uuid.hpp>

namespace svetit::space::model {

struct SpaceLink {
	boost::uuids::uuid id;
	boost::uuids::uuid spaceId;
	std::string creatorId;
	std::string name;
	int64_t createdAt;
	int64_t expiredAt;
};

} // namespace svetit::space::model