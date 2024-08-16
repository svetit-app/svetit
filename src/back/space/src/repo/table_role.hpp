#pragma once

#include "../model/role.hpp"
#include <shared/paging.hpp>
#include <shared/db/db_base.hpp>

#include <userver/components/loggable_component_base.hpp>
#include <userver/utest/using_namespace_userver.hpp>
#include <userver/storages/postgres/cluster.hpp>

namespace svetit::space::table {

class Role final {
public:
	explicit Role(std::shared_ptr<db::Base> dbPtr);
	void CreateSystemRoles();
	model::Role Select(int id, const boost::uuids::uuid& spaceId);
	int Create(const std::string& roleName, const boost::uuids::uuid& spaceId);
	void Delete(int id, const boost::uuids::uuid& spaceId);
	void Update(const model::Role& item, const boost::uuids::uuid& spaceId);
	PagingResult<model::Role> SelectList(int32_t start, int32_t limit, const boost::uuids::uuid& spaceId);

private:
	std::shared_ptr<db::Base> _db;
};

} // namespace svetit::space::table
