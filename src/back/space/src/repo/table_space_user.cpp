#include "table_space_user.hpp"

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
	role SMALLINT NOT NULL
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
	const bool& isOwner,
	int64_t joinedAt,
	const Role::Type role
	)
{
	storages::postgres::Transaction transaction =
		_pg->Begin("insert_space_user_transaction",
			storages::postgres::ClusterHostType::kMaster, {});

	transaction.Execute(kInsertSpaceUser, spaceId, userId, isOwner, joinedAt, role);
	transaction.Commit();
}

const storages::postgres::Query kDeleteBySpace {
	"DELETE FROM space_user WHERE spaceId = $1",
	storages::postgres::Query::Name{"delete_user_by_space"},
};

bool SpaceUser::DeleteBySpace(boost::uuids::uuid spaceUuid) {
	storages::postgres::Transaction transaction =
		_pg->Begin("delete_space_user_by_space_transaction",
			storages::postgres::ClusterHostType::kMaster, {});

	auto res = transaction.Execute(kDeleteBySpace, spaceUuid);

	transaction.Commit();

	return res.RowsAffected();
}

const storages::postgres::Query kIsOwner {
	"SELECT isOwner FROM space_user WHERE spaceId = $1 AND userId = $2",
	storages::postgres::Query::Name{"is_owner"},
};

bool SpaceUser::IsOwner(boost::uuids::uuid spaceUuid, std::string userId) {
	storages::postgres::Transaction transaction =
		_pg->Begin("is_owner_transaction",
			storages::postgres::ClusterHostType::kMaster, {});

	auto res = transaction.Execute(kIsOwner, spaceUuid, userId);

	bool isOwner = false;

	if (!res.IsEmpty()) {
		isOwner = res.Front()[0].As<bool>();
	}

	transaction.Commit();

	return isOwner;
}

const storages::postgres::Query kIsUserInside {
	"SELECT count(*) FROM space_user WHERE spaceId = $1 AND userId = $2",
	storages::postgres::Query::Name{"is_owner"},
};

bool SpaceUser::IsUserInside(boost::uuids::uuid spaceUuid, std::string userId) {
	storages::postgres::Transaction transaction =
		_pg->Begin("is_space_user_inside_transaction",
			storages::postgres::ClusterHostType::kMaster, {});

	auto res = transaction.Execute(kIsUserInside, spaceUuid, userId);

	bool isUserInside = false;

	if (!res.IsEmpty()) {
		const auto count = res.Front()[0].As<int64_t>();
		if (count > 0) {
			isUserInside = true;
		}
	}
	transaction.Commit();

	return isUserInside;
}

const storages::postgres::Query kGetByIds {
	"SELECT spaceId, userId, isOwner, joinedAt, role FROM space_user WHERE spaceId = $1 AND userId = $2",
	storages::postgres::Query::Name{"is_owner"},
};

model::SpaceUser SpaceUser::GetByIds(boost::uuids::uuid spaceUuid, std::string userId) {
	storages::postgres::Transaction transaction =
		_pg->Begin("get_by_ids_space_user_transaction",
			storages::postgres::ClusterHostType::kMaster, {});

	auto res = transaction.Execute(kGetByIds, spaceUuid, userId);

	if (res.IsEmpty()) {
		throw errors::BadRequest{"Not found"};
		return {};
	}

	transaction.Commit();
	return res.AsSingleRow<model::SpaceUser>(pg::kRowTag);
}

const storages::postgres::Query kGetRole {
	"SELECT role FROM space_user WHERE spaceId = $1 AND userId = $2",
	storages::postgres::Query::Name{"getRole"},
};

bool SpaceUser::IsAdmin(boost::uuids::uuid spaceUuid, std::string userId) {
	storages::postgres::Transaction transaction =
		_pg->Begin("is_admin_transaction",
			storages::postgres::ClusterHostType::kMaster, {});

	auto res = transaction.Execute(kGetRole, spaceUuid, userId);

	if (!res.IsEmpty()) {
		const auto roleStr = res.Front()[0].As<std::string>();
		const auto role = Role::FromString(roleStr);
		if (role == Role::Type::Admin) {
			return true;
		}
	}

	transaction.Commit();
	return false;
}

const storages::postgres::Query kDelete {
	"DELETE FROM space_user WHERE spaceId = $1 AND userId = $2",
	storages::postgres::Query::Name{"delete_user_by_space"},
};

bool SpaceUser::Delete(boost::uuids::uuid spaceUuid, std::string userId) {
	storages::postgres::Transaction transaction =
		_pg->Begin("delete_space_user_transaction",
			storages::postgres::ClusterHostType::kMaster, {});

	auto res = transaction.Execute(kDelete, spaceUuid, userId);

	transaction.Commit();

	return res.RowsAffected();
}

const storages::postgres::Query kUpdate {
	"UPDATE space_user SET role = $3, isOwner = $4 WHERE spaceId = $1 AND userId = $2",
	storages::postgres::Query::Name{"update_user"},
};

bool SpaceUser::Update(model::SpaceUser user) {
	storages::postgres::Transaction transaction =
		_pg->Begin("update_space_user_transaction",
			storages::postgres::ClusterHostType::kMaster, {});

	auto res = transaction.Execute(kUpdate, user.spaceId, user.userId, user.role, user.isOwner);

	transaction.Commit();

	return res.RowsAffected();
}

const storages::postgres::Query kSelectUsersInSpace{
	"SELECT spaceId, userId, isOwner, joinedAt, role FROM space_user WHERE spaceId = $1 OFFSET $2 LIMIT $3",
	storages::postgres::Query::Name{"select_users_in_space"},
};

std::vector<model::SpaceUser> SpaceUser::Get(boost::uuids::uuid spaceUuid, int start, int limit) {
	storages::postgres::Transaction transaction =
		_pg->Begin("select_users_in_space_transaction",
			storages::postgres::ClusterHostType::kMaster, {});

	auto res = transaction.Execute(kSelectUsersInSpace, spaceUuid, start, limit);
	if (res.IsEmpty())
	{
		transaction.Commit();
		return {};
	}

	transaction.Commit();
	return res.AsContainer<std::vector<model::SpaceUser>>(pg::kRowTag);
}

const storages::postgres::Query kCountBySpaceId{
	"SELECT count(*) FROM space_user WHERE spaceId = $1",
	storages::postgres::Query::Name{"count_users_by_spaceId"},
};

int SpaceUser::CountBySpaceId(const boost::uuids::uuid spaceId) {
	storages::postgres::Transaction transaction =
		_pg->Begin("count_users_by_spaceId_transaction",
			storages::postgres::ClusterHostType::kMaster, {});

	auto res = transaction.Execute(kCountBySpaceId, spaceId);

	auto count = res.Front()[0].As<int64_t>();
	transaction.Commit();

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
