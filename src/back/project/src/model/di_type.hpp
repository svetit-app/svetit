#pragma once

#include <string>
#include <boost/uuid/uuid.hpp>
#include <cstdint>

#include <shared/db/db_table_helpers.hpp>

#include "di_mode.hpp"
#include "save_algorithm.hpp"

namespace svetit::project::model {

struct DiType {
	using Ids = db::Ids<0, 1>;
	using FilterIds = db::Ids<1, 2>;

	int64_t id;
	boost::uuids::uuid spaceId;
	boost::uuids::uuid projectId;
	int64_t measureId;
	int64_t saveTimerId;
	std::string key;
	std::string name;
	DiMode::Type mode;
	SaveAlgorithm::Type saveAlgorithm;
};

} // namespace svetit::project::model