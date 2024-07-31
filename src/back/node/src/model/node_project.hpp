#pragma once

#include <boost/uuid/uuid.hpp>

namespace svetit::node::model {

struct NodeProject {
	boost::uuids::uuid nodeId;
	boost::uuids::uuid projectId;
};

} // namespace svetit::node::model