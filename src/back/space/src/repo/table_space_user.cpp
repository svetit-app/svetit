#include "table_space_user.hpp"
#include <memory>
#include <shared/errors.hpp>
#include <shared/paging.hpp>

#include <userver/components/component_config.hpp>
#include <userver/components/component_context.hpp>
#include <userver/utils/boost_uuid4.hpp>
#include <userver/yaml_config/merge_schemas.hpp>
#include <userver/storages/postgres/component.hpp>

namespace svetit::space::table {

namespace pg = storages::postgres;
using pg::ClusterHostType;

SpaceUser::SpaceUser(std::shared_ptr<db::Base> dbPtr)
	: _db{std::move(dbPtr)}
{}

const pg::Query kInsertSpaceUser{
	"INSERT INTO space.user (space_id, user_id, is_owner, role) "
	"VALUES ($1, $2, $3, $4) ",
	pg::Query::Name{"insert_space_user"},
};

void SpaceUser::Create(
	const boost::uuids::uuid& spaceId,
	const std::string& userId,
	bool isOwner,
	Role::Type role)
{
	_db->Execute(ClusterHostType::kMaster, kInsertSpaceUser, spaceId, userId, isOwner, role);
}

const pg::Query kDeleteBySpace {
	"DELETE FROM space.user WHERE space_id = $1",
	pg::Query::Name{"delete_user_by_space"},
};

void SpaceUser::DeleteBySpace(const boost::uuids::uuid& spaceId) {
	_db->Execute(ClusterHostType::kMaster, kDeleteBySpace, spaceId);
}

const pg::Query kIsOwner {
	"SELECT is_owner FROM space.user WHERE space_id = $1 AND user_id = $2",
	pg::Query::Name{"is_owner"},
};

bool SpaceUser::IsOwner(const boost::uuids::uuid& spaceId, const std::string& userId) {
	const auto res = _db->Execute(ClusterHostType::kSlave, kIsOwner, spaceId, userId);

	if (res.IsEmpty())
		throw errors::NotFound404();

	return res.AsSingleRow<bool>();
}

const pg::Query kIsUserInside {
	"SELECT COUNT(*) FROM space.user WHERE space_id = $1 AND user_id = $2",
	pg::Query::Name{"is_owner"},
};

bool SpaceUser::IsUserInside(const boost::uuids::uuid& spaceId, const std::string& userId) {
	const auto res = _db->Execute(ClusterHostType::kSlave, kIsUserInside, spaceId, userId);

	if (!res.IsEmpty()) {
		const auto count = res.AsSingleRow<int64_t>();
		if (count > 0)
			return true;
	}

	return false;
}

const pg::Query kGetByIds {
	"SELECT space_id, user_id, is_owner, joined_at, role "
	"FROM space.user WHERE space_id = $1 AND user_id = $2",
	pg::Query::Name{"get_by_ids"},
};

model::SpaceUser SpaceUser::GetByIds(const boost::uuids::uuid& spaceId, const std::string& userId) {
	auto res = _db->Execute(ClusterHostType::kSlave, kGetByIds, spaceId, userId);
	if (res.IsEmpty())
		throw errors::NotFound404{};

	return res.AsSingleRow<model::SpaceUser>(pg::kRowTag);
}

const pg::Query kGetRole {
	"SELECT role FROM space.user WHERE space_id = $1 AND user_id = $2",
	pg::Query::Name{"get_role"},
};

bool SpaceUser::IsAdmin(const boost::uuids::uuid& spaceId, const std::string& userId) {
	const auto res = _db->Execute(ClusterHostType::kSlave, kGetRole, spaceId, userId);
	if (!res.IsEmpty()) {
		const auto role = res.AsSingleRow<Role::Type>();
		if (role == Role::Type::Admin)
			return true;
	}

	return false;
}

const pg::Query kDelete {
	R"~(
		DELETE FROM space.user
		WHERE space_id = $1 AND user_id = $2 AND is_owner = false
		AND (
			user_id = $3 OR EXISTS (
				SELECT 1 FROM space.user WHERE space_id = $1 AND user_id = $3 AND role = $4
			)
		)
	)~",
	pg::Query::Name{"delete_user"},
};

void SpaceUser::Delete(const boost::uuids::uuid& spaceId, const std::string& userId, const std::string& headerUserId) {
	auto res = _db->Execute(ClusterHostType::kMaster, kDelete, spaceId, userId, headerUserId, Role::Admin);
	if (!res.RowsAffected())
		throw errors::NotFound404();
}

const pg::Query kUpdate {
	"UPDATE space.user SET role = $3, is_owner = $4 "
	"WHERE space_id = $1 AND user_id = $2",
	pg::Query::Name{"update_user"},
};

void SpaceUser::Update(const model::SpaceUser& user) {
	auto res = _db->Execute(ClusterHostType::kMaster, kUpdate, user.spaceId, user.userId, user.role, user.isOwner);
	if (!res.RowsAffected())
		throw errors::NotFound404();
}

const pg::Query kSelectUsersInSpace{
	"SELECT space_id, user_id, is_owner, joined_at, role, COUNT(*) OVER() FROM space.user "
	"WHERE space_id = $1 OFFSET $2 LIMIT $3",
	pg::Query::Name{"select_users_in_space"},
};

PagingResult<model::SpaceUser> SpaceUser::Get(const boost::uuids::uuid& spaceId, int start, int limit) {
	auto res = _db->Execute(ClusterHostType::kSlave, kSelectUsersInSpace, spaceId, start, limit);
	PagingResult<model::SpaceUser> data;
	data = res.AsContainer<decltype(data)::RawContainer>(pg::kRowTag);
	return data;
}

const pg::Query kSetIsOwner{
	"UPDATE space.user SET is_owner = $3 "
	"WHERE space_id = $1 AND user_id = $2",
	pg::Query::Name{"select_users_in_space"},
};

void SpaceUser::SetIsOwner(const boost::uuids::uuid& spaceId, const std::string& userId, bool isOwner) {
	auto res = _db->Execute(ClusterHostType::kMaster, kSetIsOwner, spaceId, userId, isOwner);
	if (!res.RowsAffected())
		throw errors::NotModified304();
}

} // namespace svetit::space::table
