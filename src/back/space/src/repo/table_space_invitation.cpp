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
{}

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

} // namespace svetit::space::table
