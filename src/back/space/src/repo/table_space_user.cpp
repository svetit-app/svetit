#include "table_space_user.hpp"
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

SpaceUser::SpaceUser(pg::ClusterPtr pg)
	: _pg{std::move(pg)}
{
	//InsertDataForMocks();
}

const pg::Query kInsertSpaceUser{
	"INSERT INTO space.user (space_id, user_id, is_owner, role) "
	"VALUES ($1, $2, $3, $4) ",
	pg::Query::Name{"insert_space_user"},
};

void SpaceUser::Insert(
	const boost::uuids::uuid& spaceId,
	const std::string& userId,
	bool isOwner,
	Role::Type role)
{
	_pg->Execute(ClusterHostType::kMaster, kInsertSpaceUser, spaceId, userId, isOwner, role);
}

const pg::Query kDeleteBySpace {
	"DELETE FROM space.user WHERE space_id = $1",
	pg::Query::Name{"delete_user_by_space"},
};

void SpaceUser::DeleteBySpace(const boost::uuids::uuid& spaceId) {
	_pg->Execute(ClusterHostType::kMaster, kDeleteBySpace, spaceId);
}

const pg::Query kIsOwner {
	"SELECT is_owner FROM space.user WHERE space_id = $1 AND user_id = $2",
	pg::Query::Name{"is_owner"},
};

bool SpaceUser::IsOwner(const boost::uuids::uuid& spaceId, const std::string& userId) {
	const auto res = _pg->Execute(ClusterHostType::kMaster, kIsOwner, spaceId, userId);

	if (res.IsEmpty())
		throw errors::NotFound404();

	return res.AsSingleRow<bool>();
}

const pg::Query kIsUserInside {
	"SELECT COUNT(*) FROM space.user WHERE space_id = $1 AND user_id = $2",
	pg::Query::Name{"is_owner"},
};

bool SpaceUser::IsUserInside(const boost::uuids::uuid& spaceId, const std::string& userId) {
	const auto res = _pg->Execute(ClusterHostType::kMaster, kIsUserInside, spaceId, userId);

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
	auto res = _pg->Execute(ClusterHostType::kMaster, kGetByIds, spaceId, userId);
	if (res.IsEmpty())
		throw errors::NotFound404{};

	return res.AsSingleRow<model::SpaceUser>(pg::kRowTag);
}

const pg::Query kGetRole {
	"SELECT role FROM space.user WHERE space_id = $1 AND user_id = $2",
	pg::Query::Name{"get_role"},
};

bool SpaceUser::IsAdmin(const boost::uuids::uuid& spaceId, const std::string& userId) {
	const auto res = _pg->Execute(ClusterHostType::kMaster, kGetRole, spaceId, userId);
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
	auto res = _pg->Execute(ClusterHostType::kMaster, kDelete, spaceId, userId, headerUserId, Role::Admin);
	if (!res.RowsAffected())
		throw errors::NotFound404();
}

const pg::Query kUpdate {
	"UPDATE space.user SET role = $3, is_owner = $4 "
	"WHERE space_id = $1 AND user_id = $2",
	pg::Query::Name{"update_user"},
};

void SpaceUser::Update(const model::SpaceUser& user) {
	auto res = _pg->Execute(ClusterHostType::kMaster, kUpdate, user.spaceId, user.userId, user.role, user.isOwner);
	if (!res.RowsAffected())
		throw errors::NotFound404();
}

const pg::Query kSelectUsersInSpace{
	"SELECT space_id, user_id, is_owner, joined_at, role FROM space.user "
	"WHERE space_id = $1 OFFSET $2 LIMIT $3",
	pg::Query::Name{"select_users_in_space"},
};

const pg::Query kCountBySpaceId{
	"SELECT COUNT(*) FROM space.user WHERE space_id = $1",
	pg::Query::Name{"count_users_by_spaceId"},
};

PagingResult<model::SpaceUser> SpaceUser::Get(const boost::uuids::uuid& spaceId, int start, int limit) {
	PagingResult<model::SpaceUser> data;

	auto trx = _pg->Begin(pg::Transaction::RO);
	auto res = trx.Execute(kSelectUsersInSpace, spaceId, start, limit);
	data.items = res.AsContainer<decltype(data.items)>(pg::kRowTag);
	res = trx.Execute(kCountBySpaceId, spaceId);
	data.total = res.AsSingleRow<int64_t>();
	trx.Commit();
	return data;
}

const pg::Query kSetIsOwner{
	"UPDATE space.user SET is_owner = $3 "
	"WHERE space_id = $1 AND user_id = $2",
	pg::Query::Name{"select_users_in_space"},
};

void SpaceUser::TransferOwnership(const boost::uuids::uuid& spaceId, const std::string& fromUserId, const std::string& toUserId) {
	auto trx = _pg->Begin(pg::Transaction::RW);
	auto res = trx.Execute(kSetIsOwner, spaceId, fromUserId, false);
	if (!res.RowsAffected())
		throw errors::NotModified304();
	res = trx.Execute(kSetIsOwner, spaceId, toUserId, true);
	if (!res.RowsAffected())
		throw errors::NotModified304();
	trx.Commit();
}

void SpaceUser::InsertDataForMocks() {
	Insert(utils::BoostUuidFromString("11111111-1111-1111-1111-111111111111"), "a1ab59c0-9770-4145-b464-abb29d52ee15", false, Role::Type::Admin);
	Insert(utils::BoostUuidFromString("11111111-1111-1111-1111-111111111111"), "c79361b1-b7f3-48d7-ad85-b1ddcdb21ee1", true, Role::Type::User);
	Insert(utils::BoostUuidFromString("11111111-1111-1111-1111-111111111111"), "3ba4505c-783d-4b24-bb82-5bd4eb193b23", false, Role::Type::Guest);
	Insert(utils::BoostUuidFromString("11111111-1111-1111-1111-111111111111"), "8689cd74-cca3-4310-8c64-96e54d03a62e", false, Role::Type::Admin);
	Insert(utils::BoostUuidFromString("11111111-1111-1111-1111-111111111111"), "019a78df-a142-41e6-a682-3900f8f37344", false, Role::Type::User);
	Insert(utils::BoostUuidFromString("11111111-1111-1111-1111-111111111111"), "e7648c4d-1911-4374-9dd7-eefede48fb31", false, Role::Type::Guest);
	Insert(utils::BoostUuidFromString("11111111-1111-1111-1111-111111111111"), "905c47ce-7f5a-4e0b-b695-535aaead45a5", false, Role::Type::Admin);
}

} // namespace svetit::space::table
