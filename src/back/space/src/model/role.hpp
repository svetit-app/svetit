#pragma once

#include <boost/uuid/uuid.hpp>
#include <string>
#include <optional>

namespace svetit::space::model {

struct Role {
	int id;
	std::optional<boost::uuids::uuid> spaceId;
	std::string name;
};

} // namespace svetit::space