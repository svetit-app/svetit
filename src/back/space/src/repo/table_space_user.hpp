#pragma once

#include "../model/space_user.hpp"
#include "../model/role.hpp"
#include <shared/paging.hpp>
#include <shared/db/db_base.hpp>

#include <userver/components/loggable_component_base.hpp>
#include <userver/utest/using_namespace_userver.hpp>
#include <userver/storages/postgres/cluster.hpp>

namespace svetit::space::table {

class SpaceUser final {
public:
	explicit SpaceUser(std::shared_ptr<db::Base> dbPtr);
	void Create(
		const boost::uuids::uuid& spaceId,
		const std::string& userId,
		bool isOwner,
		int roleId);
	void DeleteBySpace(const boost::uuids::uuid& spaceId);
	bool IsOwner(const boost::uuids::uuid& spaceId, const std::string& userId);
	bool IsUserInside(const boost::uuids::uuid& spaceId, const std::string& userId);
	model::SpaceUser GetByIds(const boost::uuids::uuid& spaceId, const std::string& userId);
	bool IsAdmin(const boost::uuids::uuid& spaceId, const std::string& userId);
	void Delete(const boost::uuids::uuid& spaceId, const std::string& userId, const std::string& headerUserId);
	void Update(const model::SpaceUser& user);
	PagingResult<model::SpaceUser> Get(const boost::uuids::uuid& spaceId, int start, int limit);
	void SetIsOwner(const boost::uuids::uuid& spaceId, const std::string& userId, bool isOwner);

private:
	std::shared_ptr<db::Base> _db;
};

} // namespace svetit::space::table
