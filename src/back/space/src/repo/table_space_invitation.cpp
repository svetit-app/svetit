#include "table_space_invitation.hpp"
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

SpaceInvitation::SpaceInvitation(pg::ClusterPtr pg)
	: _pg{std::move(pg)}
{
	//InsertDataForMocks();
}

const pg::Query kSelectSpaceInvitation{
	"SELECT id, spaceId, creatorId, userId, role, createdAt "
	"FROM space.invitation OFFSET $1 LIMIT $2",
	pg::Query::Name{"select_space.invitation"},
};

const pg::Query kCountSpaceInvitation{
	"SELECT COUNT(*) FROM space.invitation",
	pg::Query::Name{"count_space.invitation"},
};

PagingResult<model::SpaceInvitation> SpaceInvitation::Select(int offset, int limit)
{
	PagingResult<model::SpaceInvitation> data;

	auto trx = _pg->Begin(pg::Transaction::RO);
	auto res = trx.Execute(kSelectSpaceInvitation, offset, limit);
	data.items = res.AsContainer<decltype(data.items)>(pg::kRowTag);
	res = trx.Execute(kCountSpaceInvitation);
	data.total = res.AsSingleRow<int64_t>();
	trx.Commit();
	return data;
}

const pg::Query kSelectSpaceInvitationBySpace{
	"SELECT id, spaceId, creatorId, userId, role, createdAt "
	"FROM space.invitation WHERE spaceId=$1 OFFSET $2 LIMIT $3",
	pg::Query::Name{"select_space.invitation_by_space"},
};

const pg::Query kCountSpaceInvitationBySpace{
	"SELECT COUNT(*) FROM space.invitation WHERE spaceId=$1",
	pg::Query::Name{"count_space.invitation_by_space"},
};

PagingResult<model::SpaceInvitation> SpaceInvitation::SelectBySpace(const boost::uuids::uuid& spaceId, int offset, int limit)
{
	PagingResult<model::SpaceInvitation> data;

	auto trx = _pg->Begin(pg::Transaction::RO);
	auto res = trx.Execute(kSelectSpaceInvitationBySpace, spaceId, offset, limit);
	data.items = res.AsContainer<decltype(data.items)>(pg::kRowTag);
	res = trx.Execute(kCountSpaceInvitationBySpace, spaceId);
	data.total = res.AsSingleRow<int64_t>();
	trx.Commit();
	return data;
}

const pg::Query kCountInvitationsAvailable{
	R"~(
		SELECT
			(SELECT COUNT(*) FROM space.invitation WHERE creatorId != userId AND userId = $1)
			+ (SELECT COUNT(*) FROM space.invitation WHERE creatorId = userId AND userId != $1)
		AS SumCount
	)~",
	pg::Query::Name{"count_space.invitation_available"},
};

int64_t SpaceInvitation::GetAvailableCount(const std::string& currentUserId) {
	const auto res = _pg->Execute(ClusterHostType::kMaster, kCountInvitationsAvailable, currentUserId);
	if (res.IsEmpty())
		return 0;

	return res.AsSingleRow<int64_t>();;
}

const pg::Query kDeleteBySpace {
	"DELETE FROM space.invitation WHERE spaceId = $1",
	pg::Query::Name{"delete_space.invitation_by_space"},
};

void SpaceInvitation::DeleteBySpace(const boost::uuids::uuid& spaceId) {
	auto res = _pg->Execute(ClusterHostType::kMaster, kDeleteBySpace, spaceId);
	if (!res.RowsAffected())
		throw errors::NotFound();
}

const pg::Query kUpdateRole {
	"UPDATE space.invitation SET role = $1 WHERE id = $2",
	pg::Query::Name{"update_role_in_space.invitation"},
};

void SpaceInvitation::UpdateRole(int id, const Role::Type& role) {
	auto res = _pg->Execute(ClusterHostType::kMaster, kUpdateRole, role, id);
	if (!res.RowsAffected())
		throw errors::NotModified();
}

const pg::Query kSelectById{
	"SELECT id, spaceId, creatorId, userId, role, createdAt "
	"FROM space.invitation WHERE id = $1",
	pg::Query::Name{"select_space.invitation_by_id"},
};

model::SpaceInvitation SpaceInvitation::SelectById(int id)
{
	auto res = _pg->Execute(ClusterHostType::kMaster, kSelectById, id);
	if (res.IsEmpty())
		throw errors::NotFound{};

	return res.AsSingleRow<model::SpaceInvitation>(pg::kRowTag);
}

const pg::Query kDeleteById {
	"DELETE FROM space.invitation WHERE id = $1",
	pg::Query::Name{"delete_space.invitation_by_id"},
};

void SpaceInvitation::DeleteById(int id) {
	auto res = _pg->Execute(ClusterHostType::kMaster, kDeleteById, id);
	if (!res.RowsAffected())
		throw errors::NotFound{};
}

void SpaceInvitation::InsertDataForMocks() {
	const pg::Query kInsertSpaceInvitation{
		R"~(
			INSERT INTO space.invitation (spaceId, userId, role, creatorId)
			VALUES ($1, $2, $3, $4)
		)~",
		pg::Query::Name{"insert_space.invitation"},
	};

	// меня пригласили
	_pg->Execute(ClusterHostType::kMaster, kInsertSpaceInvitation, utils::BoostUuidFromString("11111111-1111-1111-1111-111111111111"), "8ad16a1d-18b1-4aaa-8b0f-f61915974c66", Role::Type::User, "01000000-0000-0000-0000-000000000000");
	_pg->Execute(ClusterHostType::kMaster, kInsertSpaceInvitation, utils::BoostUuidFromString("22222222-2222-2222-2222-222222222222"),"8ad16a1d-18b1-4aaa-8b0f-f61915974c66", Role::Type::User, "01000000-0000-0000-0000-000000000000");
	// Я прошусь
	_pg->Execute(ClusterHostType::kMaster, kInsertSpaceInvitation, utils::BoostUuidFromString("33333333-3333-3333-3333-333333333333"), "8ad16a1d-18b1-4aaa-8b0f-f61915974c66", Role::Type::Unknown, "8ad16a1d-18b1-4aaa-8b0f-f61915974c66");
	_pg->Execute(ClusterHostType::kMaster, kInsertSpaceInvitation, utils::BoostUuidFromString("44444444-4444-4444-4444-444444444444"), "8ad16a1d-18b1-4aaa-8b0f-f61915974c66", Role::Type::Unknown, "8ad16a1d-18b1-4aaa-8b0f-f61915974c66");
	// Мы пригласили
	_pg->Execute(ClusterHostType::kMaster, kInsertSpaceInvitation, utils::BoostUuidFromString("55555555-5555-5555-5555-555555555555"), "03000000-0000-0000-0000-000000000000", Role::Type::User, "04000000-0000-0000-0000-000000000000");
	_pg->Execute(ClusterHostType::kMaster, kInsertSpaceInvitation, utils::BoostUuidFromString("66666666-6666-6666-6666-666666666666"), "01000000-0000-0000-0000-000000000000", Role::Type::Guest, "04000000-0000-0000-0000-000000000000");
	// Хочет к нам
	_pg->Execute(ClusterHostType::kMaster, kInsertSpaceInvitation, utils::BoostUuidFromString("77777777-7777-7777-7777-777777777777"), "03000000-0000-0000-0000-000000000000", Role::Type::User, "03000000-0000-0000-0000-000000000000");
	_pg->Execute(ClusterHostType::kMaster, kInsertSpaceInvitation, utils::BoostUuidFromString("88888888-8888-8888-8888-888888888888"), "15000000-0000-0000-0000-000000000000", Role::Type::Guest, "15000000-0000-0000-0000-000000000000");
	_pg->Execute(ClusterHostType::kMaster, kInsertSpaceInvitation, utils::BoostUuidFromString("99999999-9999-9999-9999-999999999999"), "03000000-0000-0000-0000-000000000000", Role::Type::User, "03000000-0000-0000-0000-000000000000");
	_pg->Execute(ClusterHostType::kMaster, kInsertSpaceInvitation, utils::BoostUuidFromString("11111111-1111-1111-1111-111111111111"), "15000000-0000-0000-0000-000000000000", Role::Type::Guest, "15000000-0000-0000-0000-000000000000");
	_pg->Execute(ClusterHostType::kMaster, kInsertSpaceInvitation, utils::BoostUuidFromString("22222222-2222-2222-2222-222222222222"), "03000000-0000-0000-0000-000000000000", Role::Type::User, "03000000-0000-0000-0000-000000000000");
	_pg->Execute(ClusterHostType::kMaster, kInsertSpaceInvitation, utils::BoostUuidFromString("33333333-3333-3333-3333-333333333333"), "15000000-0000-0000-0000-000000000000", Role::Type::Guest, "15000000-0000-0000-0000-000000000000");
	_pg->Execute(ClusterHostType::kMaster, kInsertSpaceInvitation, utils::BoostUuidFromString("44444444-4444-4444-4444-444444444444"), "03000000-0000-0000-0000-000000000000", Role::Type::User, "03000000-0000-0000-0000-000000000000");
	_pg->Execute(ClusterHostType::kMaster, kInsertSpaceInvitation, utils::BoostUuidFromString("55555555-5555-5555-5555-555555555555"), "15000000-0000-0000-0000-000000000000", Role::Type::Guest, "15000000-0000-0000-0000-000000000000");
	_pg->Execute(ClusterHostType::kMaster, kInsertSpaceInvitation, utils::BoostUuidFromString("66666666-6666-6666-6666-666666666666"), "03000000-0000-0000-0000-000000000000", Role::Type::User, "03000000-0000-0000-0000-000000000000");
	_pg->Execute(ClusterHostType::kMaster, kInsertSpaceInvitation, utils::BoostUuidFromString("77777777-7777-7777-7777-777777777777"), "15000000-0000-0000-0000-000000000000", Role::Type::Guest, "15000000-0000-0000-0000-000000000000");
	// Далее данные для Space Detail Page
	// Мы пригласили
	_pg->Execute(ClusterHostType::kMaster, kInsertSpaceInvitation, utils::BoostUuidFromString("11111111-1111-1111-1111-111111111111"), "03000000-0000-0000-0000-000000000000", Role::Type::User, "04000000-0000-0000-0000-000000000000");
	_pg->Execute(ClusterHostType::kMaster, kInsertSpaceInvitation, utils::BoostUuidFromString("11111111-1111-1111-1111-111111111111"), "01000000-0000-0000-0000-000000000000", Role::Type::Guest, "04000000-0000-0000-0000-000000000000");
	// Хочет к нам
	_pg->Execute(ClusterHostType::kMaster, kInsertSpaceInvitation, utils::BoostUuidFromString("11111111-1111-1111-1111-111111111111"), "05000000-0000-0000-0000-000000000000", Role::Type::User, "05000000-0000-0000-0000-000000000000");
}

} // namespace svetit::space::table
