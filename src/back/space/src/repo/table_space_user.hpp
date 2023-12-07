#pragma once

#include "../model/space_user.hpp"
#include "../model/role.hpp"
#include "../../../shared/paging.hpp"

#include <userver/components/loggable_component_base.hpp>
#include <userver/utest/using_namespace_userver.hpp>
#include <userver/storages/postgres/cluster.hpp>

namespace svetit::space::table {

class SpaceUser final {
public:
	explicit SpaceUser(storages::postgres::ClusterPtr pg);
	void Insert(
		const boost::uuids::uuid& spaceId,
		const std::string& userId,
		bool isOwner,
		Role::Type role);
	void InsertDataForMocks();
	void DeleteBySpace(const boost::uuids::uuid& spaceUuid);
	bool IsOwner(const boost::uuids::uuid& spaceId, const std::string& userId);
	bool IsUserInside(const boost::uuids::uuid& spaceUuid, const std::string& userId);
	model::SpaceUser GetByIds(const boost::uuids::uuid& spaceUuid, const std::string& userId);
	bool IsAdmin(const boost::uuids::uuid& spaceUuid, const std::string& userId);
	void Delete(const boost::uuids::uuid& spaceUuid, const std::string& userId);
	void Update(const model::SpaceUser& user);
	PagingResult<model::SpaceUser> Get(const boost::uuids::uuid& spaceUuid, int start, int limit);

private:
	storages::postgres::ClusterPtr _pg;
};

} // namespace svetit::space::table
