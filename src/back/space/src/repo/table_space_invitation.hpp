#pragma once

#include "../model/space_invitation.hpp"
#include "../model/role.hpp"
#include <shared/paging.hpp>
#include <shared/db/db_base.hpp>

#include <userver/components/loggable_component_base.hpp>
#include <userver/utest/using_namespace_userver.hpp>
#include <userver/storages/postgres/cluster.hpp>

namespace svetit::space::table {

class SpaceInvitation final {
public:
	explicit SpaceInvitation(std::shared_ptr<db::Base> dbPtr);
	void DeleteBySpace(const boost::uuids::uuid& spaceId);
	void UpdateRole(int id, int roleId);
	model::SpaceInvitation SelectById(int id);
	void DeleteById(int id);
	bool IsUserInvited(const boost::uuids::uuid& spaceId, const std::string& userId);
private:
	std::shared_ptr<db::Base> _db;
};

} // namespace svetit::space::table
