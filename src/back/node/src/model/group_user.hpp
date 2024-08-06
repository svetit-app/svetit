#pragma once

#include <boost/uuid/uuid.hpp>
#include <string>

namespace svetit::node::model {

struct GroupUser {
	int groupId;
	std::string userId;
};

} // namespace svetit::node::model