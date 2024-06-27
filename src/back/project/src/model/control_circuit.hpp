#pragma once

#include <string>
#include <cstdint>
#include <boost/uuid/uuid.hpp>

#include <shared/db/db_table_helpers.hpp>

namespace svetit::project::model {

struct ControlCircuit {
	using Ids = db::Ids<0, 1>;
	using FilterIds = db::Ids<1, 3>;

	int64_t id;
	boost::uuids::uuid spaceId;
	int64_t typeId;
	int64_t sectionId;
	std::string name;
};

} // namespace svetit::project::model