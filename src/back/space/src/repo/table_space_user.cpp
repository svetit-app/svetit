#include "table_space_user.hpp"
#include "../../../shared/errors.hpp"
#include "../../../shared/paging.hpp"
#include <chrono>

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
	"INSERT INTO space.user (spaceId, userId, isOwner, role) "
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
	"DELETE FROM space.user WHERE spaceId = $1",
	pg::Query::Name{"delete_user_by_space"},
};

void SpaceUser::DeleteBySpace(const boost::uuids::uuid& spaceId) {
	auto res = _pg->Execute(ClusterHostType::kMaster, kDeleteBySpace, spaceId);
	if (!res.RowsAffected())
		throw errors::NotFound();
}

const pg::Query kIsOwner {
	"SELECT isOwner FROM space.user WHERE spaceId = $1 AND userId = $2",
	pg::Query::Name{"is_owner"},
};

bool SpaceUser::IsOwner(const boost::uuids::uuid& spaceId, const std::string& userId) {
	const auto res = _pg->Execute(ClusterHostType::kMaster, kIsOwner, spaceId, userId);

	if (res.IsEmpty())
		throw errors::NotFound();

	return res.AsSingleRow<bool>();
}

const pg::Query kIsUserInside {
	"SELECT COUNT(*) FROM space.user WHERE spaceId = $1 AND userId = $2",
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
	"SELECT spaceId, userId, isOwner, joinedAt, role "
	"FROM space.user WHERE spaceId = $1 AND userId = $2",
	pg::Query::Name{"is_owner"},
};

model::SpaceUser SpaceUser::GetByIds(const boost::uuids::uuid& spaceId, const std::string& userId) {
	auto res = _pg->Execute(ClusterHostType::kMaster, kGetByIds, spaceId, userId);
	if (res.IsEmpty())
		throw errors::NotFound{};

	return res.AsSingleRow<model::SpaceUser>(pg::kRowTag);
}

const pg::Query kGetRole {
	"SELECT role FROM space.user WHERE spaceId = $1 AND userId = $2",
	pg::Query::Name{"getRole"},
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
	"DELETE FROM space.user WHERE spaceId = $1 AND userId = $2 AND isOwner = false AND (userId = $3 OR EXISTS (SELECT 1 FROM space.user WHERE spaceId = $1 AND userId = $3 AND role = 3))",
	pg::Query::Name{"delete_user"},
};

void SpaceUser::Delete(const boost::uuids::uuid& spaceId, const std::string& userId, const std::string& headerUserId) {
	auto res = _pg->Execute(ClusterHostType::kMaster, kDelete, spaceId, userId, headerUserId);
	if (!res.RowsAffected())
		throw errors::NotFound();
}

const pg::Query kUpdate {
	"UPDATE space.user SET role = $3, isOwner = $4 "
	"WHERE spaceId = $1 AND userId = $2",
	pg::Query::Name{"update_user"},
};

void SpaceUser::Update(const model::SpaceUser& user) {
	auto res = _pg->Execute(ClusterHostType::kMaster, kUpdate, user.spaceId, user.userId, user.role, user.isOwner);
	if (!res.RowsAffected())
		throw errors::NotFound();
}

const pg::Query kSelectUsersInSpace{
	"SELECT spaceId, userId, isOwner, joinedAt, role FROM space.user "
	"WHERE spaceId = $1 OFFSET $2 LIMIT $3",
	pg::Query::Name{"select_users_in_space"},
};

const pg::Query kCountBySpaceId{
	"SELECT COUNT(*) FROM space.user WHERE spaceId = $1",
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
	"UPDATE space.user SET isOwner = $3 "
	"WHERE spaceId = $1 AND userId = $2",
	pg::Query::Name{"select_users_in_space"},
};

void SpaceUser::TransferOwnership(const boost::uuids::uuid& spaceId, const std::string& fromUserId, const std::string& toUserId) {
	auto trx = _pg->Begin(pg::Transaction::RW);
	auto res = trx.Execute(kSetIsOwner, spaceId, fromUserId, false);
	if (!res.RowsAffected())
		throw errors::NotModified();
	res = trx.Execute(kSetIsOwner, spaceId, toUserId, true);
	if (!res.RowsAffected())
		throw errors::NotModified();
	trx.Commit();
}

void SpaceUser::InsertDataForMocks() {
	Insert(utils::BoostUuidFromString("11111111-1111-1111-1111-111111111111"), "01d16a1d-18b1-4aaa-8b0f-f61915974c66", false, Role::Type::Admin);
	Insert(utils::BoostUuidFromString("11111111-1111-1111-1111-111111111111"), "02d16a1d-18b1-4aaa-8b0f-f61915974c66", true, Role::Type::User);
	Insert(utils::BoostUuidFromString("11111111-1111-1111-1111-111111111111"), "03d16a1d-18b1-4aaa-8b0f-f61915974c66", false, Role::Type::Guest);
	Insert(utils::BoostUuidFromString("11111111-1111-1111-1111-111111111111"), "04d16a1d-18b1-4aaa-8b0f-f61915974c66", false, Role::Type::Admin);
	Insert(utils::BoostUuidFromString("11111111-1111-1111-1111-111111111111"), "05d16a1d-18b1-4aaa-8b0f-f61915974c66", false, Role::Type::User);
	Insert(utils::BoostUuidFromString("11111111-1111-1111-1111-111111111111"), "06d16a1d-18b1-4aaa-8b0f-f61915974c66", false, Role::Type::Guest);
	Insert(utils::BoostUuidFromString("11111111-1111-1111-1111-111111111111"), "07d16a1d-18b1-4aaa-8b0f-f61915974c66", false, Role::Type::Admin);
	Insert(utils::BoostUuidFromString("11111111-1111-1111-1111-111111111111"), "08d16a1d-18b1-4aaa-8b0f-f61915974c66", false, Role::Type::User);
	Insert(utils::BoostUuidFromString("11111111-1111-1111-1111-111111111111"), "09d16a1d-18b1-4aaa-8b0f-f61915974c66", false, Role::Type::Guest);
	Insert(utils::BoostUuidFromString("11111111-1111-1111-1111-111111111111"), "10d16a1d-18b1-4aaa-8b0f-f61915974c66", false, Role::Type::Admin);
	Insert(utils::BoostUuidFromString("11111111-1111-1111-1111-111111111111"), "11d16a1d-18b1-4aaa-8b0f-f61915974c66", false, Role::Type::User);
	Insert(utils::BoostUuidFromString("11111111-1111-1111-1111-111111111111"), "12d16a1d-18b1-4aaa-8b0f-f61915974c66", false, Role::Type::Guest);
	Insert(utils::BoostUuidFromString("11111111-1111-1111-1111-111111111111"), "13d16a1d-18b1-4aaa-8b0f-f61915974c66", false, Role::Type::Admin);
	Insert(utils::BoostUuidFromString("11111111-1111-1111-1111-111111111111"), "14d16a1d-18b1-4aaa-8b0f-f61915974c66", false, Role::Type::User);
	Insert(utils::BoostUuidFromString("11111111-1111-1111-1111-111111111111"), "15d16a1d-18b1-4aaa-8b0f-f61915974c66", false, Role::Type::Guest);
}

} // namespace svetit::space::table
