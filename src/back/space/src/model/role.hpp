#pragma once

#include <shared/db/db_table_helpers.hpp>

#include <boost/uuid/uuid.hpp>
#include <string>
#include <optional>

namespace svetit::space::model {

struct Role {
	using Ids = db::Ids<0, 1>;
	using FilterIds = db::Ids<1>;

	int id;
	std::optional<boost::uuids::uuid> spaceId;
	std::string name;
};

} // namespace svetit::space
