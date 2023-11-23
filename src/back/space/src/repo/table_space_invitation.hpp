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
		const std::string& creatorId,
		const int64_t createdAt);
	PagingResult<model::SpaceInvitation> Select(const int offset, const int limit);
	int GetAvailableCount(const std::string& currentUserId);
	void InsertDataForMocks();
	void DeleteBySpace(const boost::uuids::uuid& spaceUuid);
	void UpdateRole(const int id, const Role::Type& role);
	model::SpaceInvitation SelectById(const int id);
	void DeleteById(const int id);

private:
	storages::postgres::ClusterPtr _pg;
};

} // namespace svetit::space::table
