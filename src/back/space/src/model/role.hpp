#pragma once

#include <boost/uuid/uuid.hpp>
#include <string>

namespace svetit::space {

struct Role {
	int id;
	boost::uuids::uuid spaceId;
	std::string name;
};

} // namespace svetit::space