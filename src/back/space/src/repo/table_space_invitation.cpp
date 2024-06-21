#include "table_space_invitation.hpp"
#include <shared/errors.hpp>
#include <shared/paging.hpp>

#include <memory>

#include <userver/components/component_config.hpp>
#include <userver/components/component_context.hpp>
#include <userver/utils/boost_uuid4.hpp>
#include <userver/yaml_config/merge_schemas.hpp>
#include <userver/storages/postgres/component.hpp>

namespace svetit::space::table {

namespace pg = storages::postgres;
using pg::ClusterHostType;

SpaceInvitation::SpaceInvitation(std::shared_ptr<db::Base> dbPtr)
	: _db{std::move(dbPtr)}
{
	//InsertDataForMocks();
}

const pg::Query kCountInvitationsAvailable{
	R"~(
		SELECT
			(SELECT COUNT(*) FROM space.invitation WHERE creator_id != user_id AND user_id = $1)
			+ (SELECT COUNT(*) FROM space.invitation WHERE creator_id = user_id AND user_id != $1)
		AS sum_count
	)~",
	pg::Query::Name{"count_space.invitation_available"},
};

int64_t SpaceInvitation::GetAvailableCount(const std::string& currentUserId) {
	const auto res = _db->Execute(ClusterHostType::kSlave, kCountInvitationsAvailable, currentUserId);
	if (res.IsEmpty())
		return 0;

	return res.AsSingleRow<int64_t>();;
}

const pg::Query kDeleteBySpace {
	"DELETE FROM space.invitation WHERE space_id = $1",
	pg::Query::Name{"delete_space.invitation_by_space"},
};

void SpaceInvitation::DeleteBySpace(const boost::uuids::uuid& spaceId) {
	_db->Execute(ClusterHostType::kMaster, kDeleteBySpace, spaceId);
}

const pg::Query kUpdateRole {
	"UPDATE space.invitation SET role = $1 WHERE id = $2",
	pg::Query::Name{"update_role_in_space.invitation"},
};

void SpaceInvitation::UpdateRole(int id, const Role::Type& role) {
	auto res = _db->Execute(ClusterHostType::kMaster, kUpdateRole, role, id);
	if (!res.RowsAffected())
		throw errors::NotModified304();
}

const pg::Query kSelectById{
	"SELECT id, space_id, creator_id, user_id, role, created_at "
	"FROM space.invitation WHERE id = $1",
	pg::Query::Name{"select_space.invitation_by_id"},
};

model::SpaceInvitation SpaceInvitation::SelectById(int id)
{
	auto res = _db->Execute(ClusterHostType::kSlave, kSelectById, id);
	if (res.IsEmpty())
		throw errors::NotFound404{};

	return res.AsSingleRow<model::SpaceInvitation>(pg::kRowTag);
}

const pg::Query kDeleteById {
	"DELETE FROM space.invitation WHERE id = $1",
	pg::Query::Name{"delete_space.invitation_by_id"},
};

void SpaceInvitation::DeleteById(int id) {
	auto res = _db->Execute(ClusterHostType::kMaster, kDeleteById, id);
	if (!res.RowsAffected())
		throw errors::NotFound404{};
}

void SpaceInvitation::InsertDataForMocks() {
	const pg::Query kInsertSpaceInvitation{
		R"~(
			INSERT INTO space.invitation (space_id, user_id, role, creator_id)
			VALUES ($1, $2, $3, $4)
		)~",
		pg::Query::Name{"insert_space.invitation"},
	};

	// меня пригласили
	_db->Execute(ClusterHostType::kMaster, kInsertSpaceInvitation, utils::BoostUuidFromString("11111111-1111-1111-1111-111111111111"), "38672cb0-20a5-40cc-8f73-bae0073988c0", Role::Type::User, "8fcd4fd9-3af2-410f-9501-0f9437f6ef87");
	_db->Execute(ClusterHostType::kMaster, kInsertSpaceInvitation, utils::BoostUuidFromString("22222222-2222-2222-2222-222222222222"),"38672cb0-20a5-40cc-8f73-bae0073988c0", Role::Type::User, "8fcd4fd9-3af2-410f-9501-0f9437f6ef87");
	// Я прошусь
	_db->Execute(ClusterHostType::kMaster, kInsertSpaceInvitation, utils::BoostUuidFromString("33333333-3333-3333-3333-333333333333"), "38672cb0-20a5-40cc-8f73-bae0073988c0", Role::Type::Unknown, "38672cb0-20a5-40cc-8f73-bae0073988c0");
	_db->Execute(ClusterHostType::kMaster, kInsertSpaceInvitation, utils::BoostUuidFromString("44444444-4444-4444-4444-444444444444"), "38672cb0-20a5-40cc-8f73-bae0073988c0", Role::Type::Unknown, "38672cb0-20a5-40cc-8f73-bae0073988c0");
	// Мы пригласили
	_db->Execute(ClusterHostType::kMaster, kInsertSpaceInvitation, utils::BoostUuidFromString("55555555-5555-5555-5555-555555555555"), "496445b8-22ae-400e-8a88-08ca3a7fe16b", Role::Type::User, "c08a2241-b965-40af-b133-ec07beb52ffd");
	_db->Execute(ClusterHostType::kMaster, kInsertSpaceInvitation, utils::BoostUuidFromString("66666666-6666-6666-6666-666666666666"), "8fcd4fd9-3af2-410f-9501-0f9437f6ef87", Role::Type::Guest, "c08a2241-b965-40af-b133-ec07beb52ffd");
	// Хочет к нам
	_db->Execute(ClusterHostType::kMaster, kInsertSpaceInvitation, utils::BoostUuidFromString("77777777-7777-7777-7777-777777777777"), "496445b8-22ae-400e-8a88-08ca3a7fe16b", Role::Type::User, "496445b8-22ae-400e-8a88-08ca3a7fe16b");
	_db->Execute(ClusterHostType::kMaster, kInsertSpaceInvitation, utils::BoostUuidFromString("88888888-8888-8888-8888-888888888888"), "91844100-e1bb-4d2b-ad52-11d1397f47c0", Role::Type::Guest, "91844100-e1bb-4d2b-ad52-11d1397f47c0");
	_db->Execute(ClusterHostType::kMaster, kInsertSpaceInvitation, utils::BoostUuidFromString("99999999-9999-9999-9999-999999999999"), "496445b8-22ae-400e-8a88-08ca3a7fe16b", Role::Type::User, "496445b8-22ae-400e-8a88-08ca3a7fe16b");
	_db->Execute(ClusterHostType::kMaster, kInsertSpaceInvitation, utils::BoostUuidFromString("11111111-1111-1111-1111-111111111111"), "91844100-e1bb-4d2b-ad52-11d1397f47c0", Role::Type::Guest, "91844100-e1bb-4d2b-ad52-11d1397f47c0");
	_db->Execute(ClusterHostType::kMaster, kInsertSpaceInvitation, utils::BoostUuidFromString("22222222-2222-2222-2222-222222222222"), "496445b8-22ae-400e-8a88-08ca3a7fe16b", Role::Type::User, "496445b8-22ae-400e-8a88-08ca3a7fe16b");
	_db->Execute(ClusterHostType::kMaster, kInsertSpaceInvitation, utils::BoostUuidFromString("33333333-3333-3333-3333-333333333333"), "91844100-e1bb-4d2b-ad52-11d1397f47c0", Role::Type::Guest, "91844100-e1bb-4d2b-ad52-11d1397f47c0");
	_db->Execute(ClusterHostType::kMaster, kInsertSpaceInvitation, utils::BoostUuidFromString("44444444-4444-4444-4444-444444444444"), "496445b8-22ae-400e-8a88-08ca3a7fe16b", Role::Type::User, "496445b8-22ae-400e-8a88-08ca3a7fe16b");
	_db->Execute(ClusterHostType::kMaster, kInsertSpaceInvitation, utils::BoostUuidFromString("55555555-5555-5555-5555-555555555555"), "91844100-e1bb-4d2b-ad52-11d1397f47c0", Role::Type::Guest, "91844100-e1bb-4d2b-ad52-11d1397f47c0");
	_db->Execute(ClusterHostType::kMaster, kInsertSpaceInvitation, utils::BoostUuidFromString("66666666-6666-6666-6666-666666666666"), "496445b8-22ae-400e-8a88-08ca3a7fe16b", Role::Type::User, "496445b8-22ae-400e-8a88-08ca3a7fe16b");
	_db->Execute(ClusterHostType::kMaster, kInsertSpaceInvitation, utils::BoostUuidFromString("77777777-7777-7777-7777-777777777777"), "91844100-e1bb-4d2b-ad52-11d1397f47c0", Role::Type::Guest, "91844100-e1bb-4d2b-ad52-11d1397f47c0");
	// Далее данные для Space Detail Page
	// Мы пригласили
	_db->Execute(ClusterHostType::kMaster, kInsertSpaceInvitation, utils::BoostUuidFromString("11111111-1111-1111-1111-111111111111"), "496445b8-22ae-400e-8a88-08ca3a7fe16b", Role::Type::User, "c08a2241-b965-40af-b133-ec07beb52ffd");
	_db->Execute(ClusterHostType::kMaster, kInsertSpaceInvitation, utils::BoostUuidFromString("11111111-1111-1111-1111-111111111111"), "8fcd4fd9-3af2-410f-9501-0f9437f6ef87", Role::Type::Guest, "c08a2241-b965-40af-b133-ec07beb52ffd");
	// Хочет к нам
	_db->Execute(ClusterHostType::kMaster, kInsertSpaceInvitation, utils::BoostUuidFromString("11111111-1111-1111-1111-111111111111"), "c08a2241-b965-40af-b133-ec07beb52ffd", Role::Type::User, "c08a2241-b965-40af-b133-ec07beb52ffd");
}

} // namespace svetit::space::table
