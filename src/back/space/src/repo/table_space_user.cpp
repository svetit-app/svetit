#include "table_space_user.hpp"
#include "../../../shared/errors.hpp"
#include <chrono>

#include <userver/components/component_config.hpp>
#include <userver/components/component_context.hpp>
#include <userver/utils/boost_uuid4.hpp>
#include <userver/yaml_config/merge_schemas.hpp>
#include <userver/storages/postgres/component.hpp>

namespace svetit::space::table {

namespace pg = storages::postgres;

SpaceUser::SpaceUser(storages::postgres::ClusterPtr pg)
	: _pg{std::move(pg)}
{
	constexpr auto kCreateTable = R"~(
CREATE TABLE IF NOT EXISTS space_user (
	spaceId UUID NOT NULL,
	userId TEXT NOT NULL,
	isOwner BOOLEAN NOT NULL,
	joinedAt BIGINT NOT NULL,
	role SMALLINT NOT NULL,
	PRIMARY KEY (spaceId, userId),
	FOREIGN KEY (spaceId) REFERENCES space (id)
);
)~";

	using storages::postgres::ClusterHostType;
	_pg->Execute(ClusterHostType::kMaster, kCreateTable);

	//InsertDataForMocks();
}

const storages::postgres::Query kInsertSpaceUser{
	"INSERT INTO space_user (spaceId, userId, isOwner, joinedAt, role) "
	"VALUES ($1, $2, $3, $4, $5) ",
	storages::postgres::Query::Name{"insert_space_user"},
};

void SpaceUser::Insert(
	const boost::uuids::uuid& spaceId,
	const std::string& userId,
	const bool isOwner,
	const int64_t joinedAt,
	const Role::Type& role
	)
{
	_pg->Execute(storages::postgres::ClusterHostType::kMaster, kInsertSpaceUser, spaceId, userId, isOwner, joinedAt, role);
}

const storages::postgres::Query kDeleteBySpace {
	"DELETE FROM space_user WHERE spaceId = $1",
	storages::postgres::Query::Name{"delete_user_by_space"},
};

void SpaceUser::DeleteBySpace(const boost::uuids::uuid& spaceUuid) {
	auto res = _pg->Execute(storages::postgres::ClusterHostType::kMaster, kDeleteBySpace, spaceUuid);
	if (!res.RowsAffected())
		throw errors::NotFound();
}

const storages::postgres::Query kIsOwner {
	"SELECT isOwner FROM space_user WHERE spaceId = $1 AND userId = $2",
	storages::postgres::Query::Name{"is_owner"},
};

bool SpaceUser::IsOwner(const boost::uuids::uuid& spaceUuid, const std::string& userId) {
	auto res = _pg->Execute(storages::postgres::ClusterHostType::kMaster, kIsOwner, spaceUuid, userId);

	bool isOwner = false;

	if (!res.IsEmpty()) {
		isOwner = res.Front()[0].As<bool>();
	}

	return isOwner;
}

const storages::postgres::Query kIsUserInside {
	"SELECT count(*) FROM space_user WHERE spaceId = $1 AND userId = $2",
	storages::postgres::Query::Name{"is_owner"},
};

bool SpaceUser::IsUserInside(const boost::uuids::uuid& spaceUuid, const std::string& userId) {
	auto res = _pg->Execute(storages::postgres::ClusterHostType::kMaster, kIsUserInside, spaceUuid, userId);

	bool isUserInside = false;

	if (!res.IsEmpty()) {
		const auto count = res.Front()[0].As<int64_t>();
		if (count > 0) {
			isUserInside = true;
		}
	}

	return isUserInside;
}

const storages::postgres::Query kGetByIds {
	"SELECT spaceId, userId, isOwner, joinedAt, role "
	"FROM space_user WHERE spaceId = $1 AND userId = $2",
	storages::postgres::Query::Name{"is_owner"},
};

model::SpaceUser SpaceUser::GetByIds(const boost::uuids::uuid& spaceUuid, const std::string& userId) {
	auto res = _pg->Execute(storages::postgres::ClusterHostType::kMaster, kGetByIds, spaceUuid, userId);
	if (res.IsEmpty()) {
		throw errors::NotFound{};
	}

	return res.AsSingleRow<model::SpaceUser>(pg::kRowTag);
}

const storages::postgres::Query kGetRole {
	"SELECT role FROM space_user WHERE spaceId = $1 AND userId = $2",
	storages::postgres::Query::Name{"getRole"},
};

bool SpaceUser::IsAdmin(const boost::uuids::uuid& spaceUuid, const std::string& userId) {
	auto res = _pg->Execute(storages::postgres::ClusterHostType::kMaster, kGetRole, spaceUuid, userId);
	if (!res.IsEmpty()) {
		const auto roleStr = res.Front()[0].As<std::string>();
		const auto role = Role::FromString(roleStr);
		if (role == Role::Type::Admin) {
			return true;
		}
	}

	return false;
}

const storages::postgres::Query kDelete {
	"DELETE FROM space_user WHERE spaceId = $1 AND userId = $2",
	storages::postgres::Query::Name{"delete_user_by_space"},
};

void SpaceUser::Delete(const boost::uuids::uuid& spaceUuid, const std::string& userId) {
	auto res = _pg->Execute(storages::postgres::ClusterHostType::kMaster, kDelete, spaceUuid, userId);
	if (!res.RowsAffected())
		throw errors::NotFound();
}

const storages::postgres::Query kUpdate {
	"UPDATE space_user SET role = $3, isOwner = $4 "
	"WHERE spaceId = $1 AND userId = $2",
	storages::postgres::Query::Name{"update_user"},
};

void SpaceUser::Update(const model::SpaceUser& user) {
	auto res = _pg->Execute(storages::postgres::ClusterHostType::kMaster, kUpdate, user.spaceId, user.userId, user.role, user.isOwner);
	if (!res.RowsAffected())
		throw errors::NotFound();
}

const storages::postgres::Query kSelectUsersInSpace{
	"SELECT spaceId, userId, isOwner, joinedAt, role FROM space_user "
	"WHERE spaceId = $1 OFFSET $2 LIMIT $3",
	storages::postgres::Query::Name{"select_users_in_space"},
};

std::vector<model::SpaceUser> SpaceUser::Get(const boost::uuids::uuid& spaceUuid, const int start, const int limit) {
	auto res = _pg->Execute(storages::postgres::ClusterHostType::kMaster, kSelectUsersInSpace, spaceUuid, start, limit);
	if (res.IsEmpty())
	{
		return {};
	}

	return res.AsContainer<std::vector<model::SpaceUser>>(pg::kRowTag);
}

const storages::postgres::Query kCountBySpaceId{
	"SELECT count(*) FROM space_user WHERE spaceId = $1",
	storages::postgres::Query::Name{"count_users_by_spaceId"},
};

int SpaceUser::CountBySpaceId(const boost::uuids::uuid& spaceId) {
	auto res = _pg->Execute(storages::postgres::ClusterHostType::kMaster, kCountBySpaceId, spaceId);

	auto count = res.Front()[0].As<int64_t>();

	return count;
}

void SpaceUser::InsertDataForMocks() {
	const auto p1 = std::chrono::system_clock::now();
	const auto now = std::chrono::duration_cast<std::chrono::seconds>(p1.time_since_epoch()).count();
	Insert(utils::BoostUuidFromString("11111111-1111-1111-1111-111111111111"), "01d16a1d-18b1-4aaa-8b0f-f61915974c66", false, now, Role::Type::Admin);
	Insert(utils::BoostUuidFromString("11111111-1111-1111-1111-111111111111"), "02d16a1d-18b1-4aaa-8b0f-f61915974c66", true, now, Role::Type::User);
	Insert(utils::BoostUuidFromString("11111111-1111-1111-1111-111111111111"), "03d16a1d-18b1-4aaa-8b0f-f61915974c66", false, now, Role::Type::Guest);
	Insert(utils::BoostUuidFromString("11111111-1111-1111-1111-111111111111"), "04d16a1d-18b1-4aaa-8b0f-f61915974c66", false, now, Role::Type::Admin);
	Insert(utils::BoostUuidFromString("11111111-1111-1111-1111-111111111111"), "05d16a1d-18b1-4aaa-8b0f-f61915974c66", false, now, Role::Type::User);
	Insert(utils::BoostUuidFromString("11111111-1111-1111-1111-111111111111"), "06d16a1d-18b1-4aaa-8b0f-f61915974c66", false, now, Role::Type::Guest);
	Insert(utils::BoostUuidFromString("11111111-1111-1111-1111-111111111111"), "07d16a1d-18b1-4aaa-8b0f-f61915974c66", false, now, Role::Type::Admin);
	Insert(utils::BoostUuidFromString("11111111-1111-1111-1111-111111111111"), "08d16a1d-18b1-4aaa-8b0f-f61915974c66", false, now, Role::Type::User);
	Insert(utils::BoostUuidFromString("11111111-1111-1111-1111-111111111111"), "09d16a1d-18b1-4aaa-8b0f-f61915974c66", false, now, Role::Type::Guest);
	Insert(utils::BoostUuidFromString("11111111-1111-1111-1111-111111111111"), "10d16a1d-18b1-4aaa-8b0f-f61915974c66", false, now, Role::Type::Admin);
	Insert(utils::BoostUuidFromString("11111111-1111-1111-1111-111111111111"), "11d16a1d-18b1-4aaa-8b0f-f61915974c66", false, now, Role::Type::User);
	Insert(utils::BoostUuidFromString("11111111-1111-1111-1111-111111111111"), "12d16a1d-18b1-4aaa-8b0f-f61915974c66", false, now, Role::Type::Guest);
	Insert(utils::BoostUuidFromString("11111111-1111-1111-1111-111111111111"), "13d16a1d-18b1-4aaa-8b0f-f61915974c66", false, now, Role::Type::Admin);
	Insert(utils::BoostUuidFromString("11111111-1111-1111-1111-111111111111"), "14d16a1d-18b1-4aaa-8b0f-f61915974c66", false, now, Role::Type::User);
	Insert(utils::BoostUuidFromString("11111111-1111-1111-1111-111111111111"), "15d16a1d-18b1-4aaa-8b0f-f61915974c66", false, now, Role::Type::Guest);
}

} // namespace svetit::space::table
