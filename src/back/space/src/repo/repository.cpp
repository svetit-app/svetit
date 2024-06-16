#include "repository.hpp"

#include "shared/db/db_base.hpp"
#include "table_space.hpp"
#include "table_space_user.hpp"
#include "table_space_invitation.hpp"
#include "table_space_link.hpp"
#include "userver/storages/postgres/postgres_fwd.hpp"
#include <memory>
#include <shared/errors.hpp>

#include <userver/components/component_config.hpp>
#include <userver/components/component_context.hpp>
#include <userver/yaml_config/merge_schemas.hpp>
#include <userver/storages/postgres/component.hpp>

namespace svetit::space {

namespace pg = storages::postgres;
using pg::ClusterHostType;

/*static*/ yaml_config::Schema RepositoryComponent::GetStaticConfigSchema()
{
	return yaml_config::MergeSchemas<components::LoggableComponentBase>(R"(
type: object
description: Main repository component
additionalProperties: false
properties:
  some-var:
    type: string
    description: some desc
)");
}

RepositoryComponent::RepositoryComponent(
		const components::ComponentConfig& conf,
		const components::ComponentContext& ctx)
	: components::LoggableComponentBase{conf, ctx}
	, Repository{ctx.FindComponent<components::Postgres>("database").GetCluster()}
{}

Repository::Repository(storages::postgres::ClusterPtr pg)
	: Repository{std::make_shared<db::Base>(std::move(pg))} {}

Repository::Repository(std::shared_ptr<db::Base> dbPtr)
	: _db{std::move(dbPtr)}
	, _space{_db}
	, _spaceUser{_db}
	, _spaceInvitation{_db}
	, _spaceLink{_db}
{}

table::Space& Repository::Space() {
	return _space;
}

table::SpaceUser& Repository::SpaceUser() {
	return _spaceUser;
}

table::SpaceInvitation& Repository::SpaceInvitation() {
	return _spaceInvitation;
}

table::SpaceLink& Repository::SpaceLink() {
	return _spaceLink;
}

Repository Repository::WithTrx(const pg::TransactionOptions& opt) {
	return Repository{std::make_shared<db::Base>(_db->WithTrx(opt))};
}

void Repository::Commit() {
	_db->Commit();
}

const pg::Query kSelectSpaceAvailable{
	R"~(
		SELECT s.id, s.name, s.key, s.requests_allowed, s.created_at
		FROM space.space s
		LEFT JOIN space.user su ON s.id = su.space_id AND su.user_id = $1
		WHERE s.requests_allowed = true AND su.space_id IS NULL
		OFFSET $2 LIMIT $3;
	)~",
	pg::Query::Name{"select_space_available"},
};

const pg::Query kCountSpaceAvailable{
	R"~(
		SELECT COUNT(*)
		FROM space.space s
		LEFT JOIN space.user su ON s.id = su.space_id AND su.user_id = $1
		WHERE s.requests_allowed = true AND su.space_id IS NULL;
	)~",
	pg::Query::Name{"count_space_available"},
};

PagingResult<model::Space> Repository::SelectAvailable(const std::string& userId, int offset, int limit)
{
	PagingResult<model::Space> data;

	auto trx = _db->Begin(pg::Transaction::RO);
	auto res = trx.Execute(kSelectSpaceAvailable, userId, offset, limit);
	data.items = res.AsContainer<decltype(data.items)>(pg::kRowTag);
	res = trx.Execute(kCountSpaceAvailable, userId);
	data.total = res.AsSingleRow<int64_t>();
	trx.Commit();
	return data;
}

const pg::Query kSelectSpaceAvailableBySpaceName{
	R"~(
		SELECT s.id, s.name, s.key, s.requests_allowed, s.created_at
		FROM space.space s
		LEFT JOIN space.user su ON s.id = su.space_id AND su.user_id = $1
		WHERE s.requests_allowed = true AND su.space_id IS NULL AND s.name LIKE $2
		OFFSET $3 LIMIT $4;
	)~",
	pg::Query::Name{"select_space_available_by_space_name"},
};

const pg::Query kCountSpaceAvailableBySpaceName{
	R"~(
		SELECT COUNT(*)
		FROM space.space s
		LEFT JOIN space.user su ON s.id = su.space_id AND su.user_id = $1
		WHERE s.requests_allowed = true AND su.space_id IS NULL AND s.name LIKE $2;
	)~",
	pg::Query::Name{"count_space_available_by_space_name"},
};

PagingResult<model::Space> Repository::SelectAvailableBySpaceName(const std::string& spaceName, const std::string& userId, int offset, int limit)
{
	PagingResult<model::Space> data;

	auto trx = _db->Begin(pg::Transaction::RO);
	auto res = trx.Execute(kSelectSpaceAvailableBySpaceName, userId, '%' + spaceName + '%', offset, limit);
	data.items = res.AsContainer<decltype(data.items)>(pg::kRowTag);
	res = trx.Execute(kCountSpaceAvailableBySpaceName, userId, '%' + spaceName + '%');
	data.total = res.AsSingleRow<int64_t>();
	trx.Commit();
	return data;
}

const pg::Query kSelectByUserId{
	R"~(
		SELECT s.id, s.name, s.key, s.requests_allowed, s.created_at
		FROM space.space s
		LEFT JOIN space.user su ON s.id = su.space_id
		WHERE su.user_id = $1
		OFFSET $2 LIMIT $3
	)~",
	pg::Query::Name{"select_space_by_user_id"},
};

const pg::Query kCountByUserId{
	R"~(
		SELECT COUNT(*)
		FROM space.space s
		LEFT JOIN space.user su ON s.id = su.space_id
		WHERE su.user_id = $1
	)~",
	pg::Query::Name{"count_space_by_user_id"},
};

PagingResult<model::Space> Repository::SelectByUserId(const std::string& userId, int offset, int limit)
{
	PagingResult<model::Space> data;

	auto trx = _db->Begin(pg::Transaction::RO);
	auto res = trx.Execute(kSelectByUserId, userId, offset, limit);
	data.items = res.AsContainer<decltype(data.items)>(pg::kRowTag);
	res = trx.Execute(kCountByUserId, userId);
	data.total = res.AsSingleRow<int64_t>();
	trx.Commit();
	return data;
}

const pg::Query kSelectWithDateClauseForOwner {
	R"~(
		SELECT 1
		FROM space.space s
		LEFT JOIN space.user su ON s.id = su.space_id
		WHERE s.created_at >= $1 AND su.user_id = $2 AND su.is_owner = true
	)~",
	pg::Query::Name{"select_space_with_date_clause_for_owner"},
};

bool Repository::IsReadyForCreationByTime(const std::string& userId) {
	const auto minuteAgoTimestamp = std::chrono::system_clock::now() - std::chrono::minutes(1);

	auto res = _db->Execute(ClusterHostType::kMaster, kSelectWithDateClauseForOwner, minuteAgoTimestamp, userId);
	return res.IsEmpty();
}

const pg::Query kCountSpacesWithUser {
	R"~(
		SELECT COUNT(*)
		FROM space.space s
		LEFT JOIN space.user su ON s.id = su.space_id
		WHERE su.user_id = $1
	)~",
	pg::Query::Name{"count_spaces_with_user"},
};

int64_t Repository::GetCountSpacesWithUser(const std::string& userId) {
	const auto res = _db->Execute(ClusterHostType::kMaster, kCountSpacesWithUser, userId);
	if (res.IsEmpty())
		return 0;

	return res.AsSingleRow<int64_t>();
}

const pg::Query kSelectSpaceInvitation{
	R"~(
		SELECT si.id, si.space_id, si.creator_id, si.user_id, si.role, si.created_at
		FROM space.invitation si
		WHERE si.user_id = $1 OR si.space_id IN (
			SELECT su.space_id FROM space.user su WHERE su.user_id = $1 AND su.role = 3
		) OFFSET $2 LIMIT $3
	)~",
	pg::Query::Name{"select_space.invitation"},
};

const pg::Query kCountSpaceInvitation{
	R"~(
		SELECT COUNT(*)
		FROM space.invitation si
		WHERE si.user_id = $1 OR si.space_id IN (
			SELECT su.space_id FROM space.user su WHERE su.user_id = $1 AND su.role = 3
		)
	)~",
	pg::Query::Name{"count_space.invitation"},
};

PagingResult<model::SpaceInvitation> Repository::SelectInvitations(const std::string& userId, int start, int limit) {
	PagingResult<model::SpaceInvitation> data;

	auto trx = _db->Begin(pg::Transaction::RO);
	auto res = trx.Execute(kSelectSpaceInvitation, userId, start, limit);
	data.items = res.AsContainer<decltype(data.items)>(pg::kRowTag);
	res = trx.Execute(kCountSpaceInvitation, userId);
	data.total = res.AsSingleRow<int64_t>();
	trx.Commit();
	return data;
}

const pg::Query kSelectSpaceInvitationsBySpace{
	R"~(
		SELECT si.id, si.space_id, si.creator_id, si.user_id, si.role, si.created_at
		FROM space.invitation si
		WHERE si.space_id = $1 AND (si.user_id = $2 OR EXISTS (
			SELECT su.space_id FROM space.user su WHERE su.space_id = $1 AND su.user_id = $2 AND su.role = 3
		)) OFFSET $3 LIMIT $4
	)~",
    pg::Query::Name{"select_space.invitation_by_space"},
};

const pg::Query kCountSpaceInvitationsBySpace{
	R"~(
		SELECT COUNT(*)
		FROM space.invitation si
		WHERE si.space_id = $1 AND (si.user_id = $2 OR EXISTS (
			SELECT su.space_id FROM space.user su WHERE su.space_id = $1 AND su.user_id = $2 AND su.role = 3
		))
	)~",
	pg::Query::Name{"select_space.invitation_by_space"},
};

PagingResult<model::SpaceInvitation> Repository::SelectInvitationsBySpace(const boost::uuids::uuid& spaceId, const std::string& userId, int start, int limit) {
	PagingResult<model::SpaceInvitation> data;

	auto trx = _db->Begin(pg::Transaction::RO);
	auto res = trx.Execute(kSelectSpaceInvitationsBySpace, spaceId, userId, start, limit);
	data.items = res.AsContainer<decltype(data.items)>(pg::kRowTag);
	res = trx.Execute(kCountSpaceInvitationsBySpace, spaceId, userId);
	data.total = res.AsSingleRow<int64_t>();
	trx.Commit();
	return data;
}

const pg::Query kSelectByLink{
	"SELECT id, name, key, requests_allowed, created_at FROM space.space WHERE id = (SELECT space_id FROM space.link WHERE id = $1)",
	pg::Query::Name{"select_by_link"},
};

model::Space Repository::SelectByLink(const boost::uuids::uuid& link) {
	const auto res = _db->Execute(ClusterHostType::kMaster, kSelectByLink, link);
	if (res.IsEmpty())
		throw errors::NotFound404{};

	return res.AsSingleRow<model::Space>(pg::kRowTag);
}

const pg::Query kSelectSpaceLinkList{
	R"~(
		SELECT sl.id, sl.space_id, sl.creator_id, sl.name, sl.created_at, sl.expired_at
		FROM space.link sl
		WHERE sl.space_id IN (
			SELECT su.space_id FROM space.user su WHERE su.user_id = $1
		) OFFSET $2 LIMIT $3
	)~",
	pg::Query::Name{"select_space.link_list"},
};

const pg::Query kCountSpaceLinks{
	R"~(
		SELECT COUNT(*)
		FROM space.link sl
		WHERE sl.space_id IN (
			SELECT su.space_id FROM space.user su WHERE su.user_id = $1
		)
	)~",
	pg::Query::Name{"count_space.links"},
};

PagingResult<model::SpaceLink> Repository::SelectSpaceLinkList(const std::string& userId, int offset, int limit)
{
	PagingResult<model::SpaceLink> data;

	auto trx = _db->Begin(pg::Transaction::RO);
	auto res = trx.Execute(kSelectSpaceLinkList, userId, offset, limit);
	data.items = res.AsContainer<decltype(data.items)>(pg::kRowTag);
	res = trx.Execute(kCountSpaceLinks, userId);
	data.total = res.AsSingleRow<int64_t>();
	trx.Commit();
	return data;
}

const pg::Query kInsertSpaceInvitation{
	R"~(
		INSERT INTO space.invitation (space_id, user_id, role, creator_id)
		(SELECT $1, $2, $3, $4
		WHERE EXISTS (
			SELECT 1 FROM space.space s
			LEFT JOIN space.user u ON s.id = u.space_id AND u.user_id=$4
			WHERE s.id=$1 AND (s.requests_allowed OR u.role=$5)
		)) RETURNING id
	)~",
	pg::Query::Name{"insert_space.invitation"},
};

void Repository::CreateInvitation(
	const boost::uuids::uuid& spaceId,
	const std::string& userId,
	const Role::Type& role,
	const std::string& creatorId)
{
	const auto res = _db->Execute(ClusterHostType::kMaster, kInsertSpaceInvitation, spaceId, userId, role, creatorId, Role::Admin);
	if (res.IsEmpty())
		throw errors::BadRequest400("Nothing was inserted");
}

} // namespace svetit::space
