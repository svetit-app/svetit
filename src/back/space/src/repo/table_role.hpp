#pragma once

#include "../model/role.hpp"
#include <shared/db/db_table.hpp>

namespace svetit::space::table {

class Role final : public db::Table<model::Role> {
public:
	using db::Table<model::Role>::Table;

	void CreateSystemRoles();

	model::Role Get(int id, const boost::uuids::uuid& spaceId);
	PagingResult<model::Role> GetList(int32_t start, int32_t limit, const boost::uuids::uuid& spaceId);
};

} // namespace svetit::space::table
