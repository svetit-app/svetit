#pragma once

#include <boost/uuid/uuid.hpp>

namespace svetit::node::model {

struct NodeGroup {
	boost::uuids::uuid nodeId;
	int groupId;
};

} // namespace svetit::node::model