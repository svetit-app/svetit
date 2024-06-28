#pragma once

#include "../model/project.hpp"
#include <shared/db/db_table.hpp>

namespace svetit::project::table {

class Project final : public db::Table<model::Project> {
public:
	using db::Table<model::Project>::Table;
	model::Project GetByKey(const boost::uuids::uuid& spaceId, const std::string& key);
};

} // namespace svetit::project::table
