#include "table_space_invitation.hpp"
#include "../../../shared/errors.hpp"
#include <chrono>

#include <userver/components/component_config.hpp>
#include <userver/components/component_context.hpp>
#include <userver/utils/boost_uuid4.hpp>
#include <userver/yaml_config/merge_schemas.hpp>
#include <userver/storages/postgres/component.hpp>

namespace svetit::space::table {

namespace pg = storages::postgres;

SpaceInvitation::SpaceInvitation(storages::postgres::ClusterPtr pg)
	: _pg{std::move(pg)}
{
	constexpr auto kCreateTable = R"~(
CREATE TABLE IF NOT EXISTS space_invitation (
	id SERIAL PRIMARY KEY,
	spaceId UUID NOT NULL,
	creatorId TEXT NOT NULL,
	userId TEXT NOT NULL,
	role SMALLINT,
	createdAt BIGINT NOT NULL,
	FOREIGN KEY (spaceId) REFERENCES space (id)
);
)~";
// todo - is role may be null?

	using storages::postgres::ClusterHostType;
	_pg->Execute(ClusterHostType::kMaster, kCreateTable);

	//InsertDataForMocks();
}

const storages::postgres::Query kInsertSpaceInvitation{
	"INSERT INTO space_invitation (spaceId, userId, role, creatorId, createdAt) "
	"VALUES ($1, $2, $3, $4, $5) ",
	storages::postgres::Query::Name{"insert_space_invitation"},
};

void SpaceInvitation::Insert(
	const boost::uuids::uuid& spaceId,
	const std::string& userId,
	const Role::Type& role,
	const std::string& creatorId,
	const int64_t createdAt)
{
	_pg->Execute(storages::postgres::ClusterHostType::kMaster, kInsertSpaceInvitation, spaceId, userId, role, creatorId, createdAt);
}

const storages::postgres::Query kSelectSpaceInvitation{
	"SELECT id, spaceId, creatorId, userId, role, createdAt "
	"FROM space_invitation OFFSET $1 LIMIT $2",
	storages::postgres::Query::Name{"select_space_invitation"},
};

std::vector<model::SpaceInvitation> SpaceInvitation::Select(const int offset, const int limit)
{
	auto res = _pg->Execute(storages::postgres::ClusterHostType::kMaster, kSelectSpaceInvitation, offset, limit);
	if (res.IsEmpty())
		return {};

	return res.AsContainer<std::vector<model::SpaceInvitation>>(pg::kRowTag);
}

const storages::postgres::Query kCountSpaceInvitation{
	"SELECT count(*) FROM space_invitation",
	storages::postgres::Query::Name{"count_space_invitation"},
};

int SpaceInvitation::Count() {
	const auto res = _pg->Execute(storages::postgres::ClusterHostType::kMaster, kCountSpaceInvitation);

	int64_t id;
	if (!res.IsEmpty())
		id = res.Front()[0].As<int64_t>();

	return id;
}

const storages::postgres::Query kCountInvitationsAvailable{
	R"~(
		SELECT
			(SELECT count(*) FROM space_invitation WHERE creatorId != userId AND userId = $1)
			+ (SELECT count(*) FROM space_invitation WHERE creatorId = userId AND userId != $1)
		AS SumCount
	)~",
	storages::postgres::Query::Name{"count_space_invitation_available"},
};

int SpaceInvitation::GetAvailableCount(const std::string& currentUserId) {
	const auto res = _pg->Execute(storages::postgres::ClusterHostType::kMaster, kCountInvitationsAvailable, currentUserId);

	int64_t count;
	if (!res.IsEmpty())
		count = res.Front()[0].As<int64_t>();

	return count;
}

const storages::postgres::Query kDeleteBySpace {
	"DELETE FROM space_invitation WHERE spaceId = $1",
	storages::postgres::Query::Name{"delete_space_invitation_by_space"},
};

void SpaceInvitation::DeleteBySpace(const boost::uuids::uuid& spaceUuid) {
	auto res = _pg->Execute(storages::postgres::ClusterHostType::kMaster, kDeleteBySpace, spaceUuid);
	if (!res.RowsAffected())
		throw errors::NotFound();
}

const storages::postgres::Query kUpdateRole {
	"UPDATE space_invitation SET role = $1 WHERE id = $2",
	storages::postgres::Query::Name{"update_role_in_space_invitation"},
};

void SpaceInvitation::UpdateRole(const int id, const Role::Type& role) {
	auto res = _pg->Execute(storages::postgres::ClusterHostType::kMaster, kUpdateRole, role, id);
	if (!res.RowsAffected())
		throw errors::NotModified();
}

const storages::postgres::Query kSelectById{
	"SELECT * FROM space_invitation WHERE id = $1",
	storages::postgres::Query::Name{"select_space_invitation_by_id"},
};

model::SpaceInvitation SpaceInvitation::SelectById(const int id)
{
	auto res = _pg->Execute(storages::postgres::ClusterHostType::kMaster, kSelectById, id);
	if (res.IsEmpty())
		throw errors::NotFound{};

	return res.AsSingleRow<model::SpaceInvitation>(pg::kRowTag);
}

const storages::postgres::Query kDeleteById {
	"DELETE FROM space_invitation WHERE id = $1",
	storages::postgres::Query::Name{"delete_space_invitation_by_id"},
};

void SpaceInvitation::DeleteById(const int id) {
	auto res = _pg->Execute(storages::postgres::ClusterHostType::kMaster, kDeleteById, id);
	if (!res.RowsAffected())
		throw errors::NotFound{};
}

void SpaceInvitation::InsertDataForMocks() {
	const auto p1 = std::chrono::system_clock::now();
	const auto now = std::chrono::duration_cast<std::chrono::seconds>(p1.time_since_epoch()).count();
	// insert test data
	// меня пригласили
	Insert(utils::BoostUuidFromString("22222222-2222-2222-2222-222222222222"), "8ad16a1d-18b1-4aaa-8b0f-f61915974c66", Role::Type::User, "01000000-0000-0000-0000-000000000000", now);
	Insert(utils::BoostUuidFromString("22222222-2222-2222-2222-222222222222"),"8ad16a1d-18b1-4aaa-8b0f-f61915974c66", Role::Type::User, "01000000-0000-0000-0000-000000000000", now);
	// Я прошусь
	Insert(utils::BoostUuidFromString("33333333-3333-3333-3333-333333333333"), "8ad16a1d-18b1-4aaa-8b0f-f61915974c66", Role::Type::Unknown, "8ad16a1d-18b1-4aaa-8b0f-f61915974c66", now);
	Insert(utils::BoostUuidFromString("44444444-4444-4444-4444-444444444444"), "8ad16a1d-18b1-4aaa-8b0f-f61915974c66", Role::Type::Unknown, "8ad16a1d-18b1-4aaa-8b0f-f61915974c66", now);
	// Мы пригласили
	Insert(utils::BoostUuidFromString("55555555-5555-5555-5555-555555555555"), "03000000-0000-0000-0000-000000000000", Role::Type::User, "04000000-0000-0000-0000-000000000000", now);
	Insert(utils::BoostUuidFromString("66666666-6666-6666-6666-666666666666"), "01000000-0000-0000-0000-000000000000", Role::Type::Guest, "04000000-0000-0000-0000-000000000000", now);
	// Хочет к нам
	Insert(utils::BoostUuidFromString("77777777-7777-7777-7777-777777777777"), "03000000-0000-0000-0000-000000000000", Role::Type::User, "03000000-0000-0000-0000-000000000000", now);
	Insert(utils::BoostUuidFromString("88888888-8888-8888-8888-888888888888"), "15000000-0000-0000-0000-000000000000", Role::Type::Guest, "15000000-0000-0000-0000-000000000000", now);
	Insert(utils::BoostUuidFromString("99999999-9999-9999-9999-999999999999"), "03000000-0000-0000-0000-000000000000", Role::Type::User, "03000000-0000-0000-0000-000000000000", now);
	Insert(utils::BoostUuidFromString("11111111-1111-1111-1111-111111111111"), "15000000-0000-0000-0000-000000000000", Role::Type::Guest, "15000000-0000-0000-0000-000000000000", now);
	Insert(utils::BoostUuidFromString("22222222-2222-2222-2222-222222222222"), "03000000-0000-0000-0000-000000000000", Role::Type::User, "03000000-0000-0000-0000-000000000000", now);
	Insert(utils::BoostUuidFromString("33333333-3333-3333-3333-333333333333"), "15000000-0000-0000-0000-000000000000", Role::Type::Guest, "15000000-0000-0000-0000-000000000000", now);
	Insert(utils::BoostUuidFromString("44444444-4444-4444-4444-444444444444"), "03000000-0000-0000-0000-000000000000", Role::Type::User, "03000000-0000-0000-0000-000000000000", now);
	Insert(utils::BoostUuidFromString("55555555-5555-5555-5555-555555555555"), "15000000-0000-0000-0000-000000000000", Role::Type::Guest, "15000000-0000-0000-0000-000000000000", now);
	Insert(utils::BoostUuidFromString("66666666-6666-6666-6666-666666666666"), "03000000-0000-0000-0000-000000000000", Role::Type::User, "03000000-0000-0000-0000-000000000000", now);
	Insert(utils::BoostUuidFromString("77777777-7777-7777-7777-777777777777"), "15000000-0000-0000-0000-000000000000", Role::Type::Guest, "15000000-0000-0000-0000-000000000000", now);
	// Далее данные для Space Detail Page
	// Мы пригласили
	Insert(utils::BoostUuidFromString("11111111-1111-1111-1111-111111111111"), "03000000-0000-0000-0000-000000000000", Role::Type::User, "04000000-0000-0000-0000-000000000000", now);
	Insert(utils::BoostUuidFromString("11111111-1111-1111-1111-111111111111"), "01000000-0000-0000-0000-000000000000", Role::Type::Guest, "04000000-0000-0000-0000-000000000000", now);
	// Хочет к нам
	Insert(utils::BoostUuidFromString("11111111-1111-1111-1111-111111111111"), "03000000-0000-0000-0000-000000000000", Role::Type::User, "03000000-0000-0000-0000-000000000000", now);
	Insert(utils::BoostUuidFromString("11111111-1111-1111-1111-111111111111"), "15000000-0000-0000-0000-000000000000", Role::Type::Guest, "15000000-0000-0000-0000-000000000000", now);
	Insert(utils::BoostUuidFromString("11111111-1111-1111-1111-111111111111"), "03000000-0000-0000-0000-000000000000", Role::Type::User, "03000000-0000-0000-0000-000000000000", now);
	Insert(utils::BoostUuidFromString("11111111-1111-1111-1111-111111111111"), "15000000-0000-0000-0000-000000000000", Role::Type::Guest, "15000000-0000-0000-0000-000000000000", now);
	Insert(utils::BoostUuidFromString("11111111-1111-1111-1111-111111111111"), "03000000-0000-0000-0000-000000000000", Role::Type::User, "03000000-0000-0000-0000-000000000000", now);
	Insert(utils::BoostUuidFromString("11111111-1111-1111-1111-111111111111"), "15000000-0000-0000-0000-000000000000", Role::Type::Guest, "15000000-0000-0000-0000-000000000000", now);
	Insert(utils::BoostUuidFromString("11111111-1111-1111-1111-111111111111"), "03000000-0000-0000-0000-000000000000", Role::Type::User, "03000000-0000-0000-0000-000000000000", now);
	Insert(utils::BoostUuidFromString("11111111-1111-1111-1111-111111111111"), "15000000-0000-0000-0000-000000000000", Role::Type::Guest, "15000000-0000-0000-0000-000000000000", now);
	Insert(utils::BoostUuidFromString("11111111-1111-1111-1111-111111111111"), "03000000-0000-0000-0000-000000000000", Role::Type::User, "03000000-0000-0000-0000-000000000000", now);
	Insert(utils::BoostUuidFromString("11111111-1111-1111-1111-111111111111"), "15000000-0000-0000-0000-000000000000", Role::Type::Guest, "15000000-0000-0000-0000-000000000000", now);
}

} // namespace svetit::space::table