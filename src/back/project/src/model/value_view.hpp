#pragma once

#include <string>
#include <cstdint>
#include <boost/uuid/uuid.hpp>

namespace svetit::project::model {

struct ValueView {
	int64_t id;
	boost::uuids::uuid spaceId;
	int64_t diTypeId;
	std::string value;
	std::string view;
};

} // namespace svetit::project::model