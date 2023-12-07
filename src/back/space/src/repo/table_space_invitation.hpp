#pragma once

#include "../model/space_invitation.hpp"
#include "../model/role.hpp"
#include "../../../shared/paging.hpp"

#include <userver/components/loggable_component_base.hpp>
#include <userver/utest/using_namespace_userver.hpp>
#include <userver/storages/postgres/cluster.hpp>

namespace svetit::space::table {

class SpaceInvitation final {
public:
	explicit SpaceInvitation(storages::postgres::ClusterPtr pg);
	void Insert(
		const boost::uuids::uuid& spaceId,
		const std::string& userId,
		const Role::Type& role,
		const std::string& creatorId);
	PagingResult<model::SpaceInvitation> Select(int offset, int limit);
	PagingResult<model::SpaceInvitation> SelectBySpace(const boost::uuids::uuid& spaceId, int offset, int limit);
	int64_t GetAvailableCount(const std::string& currentUserId);
	void InsertDataForMocks();
	void DeleteBySpace(const boost::uuids::uuid& spaceId);
	void UpdateRole(int id, const Role::Type& role);
	model::SpaceInvitation SelectById(int id);
	void DeleteById(int id);

private:
	storages::postgres::ClusterPtr _pg;
};

} // namespace svetit::space::table
