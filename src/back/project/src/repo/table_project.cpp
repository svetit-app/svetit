#include "table_project.hpp"
#include <shared/errors.hpp>
#include <shared/paging.hpp>

#include <userver/components/component_config.hpp>
#include <userver/components/component_context.hpp>
#include <userver/yaml_config/merge_schemas.hpp>
#include <userver/storages/postgres/component.hpp>

namespace svetit::project::table {

namespace pg = storages::postgres;
using pg::ClusterHostType;

Project::Project(pg::ClusterPtr pg)
	: _pg{std::move(pg)}
{}

const pg::Query kSelect{
	"SELECT id, space_id, key, name, description, changed_at, sync FROM project.project WHERE id = $1",
	pg::Query::Name{"select_project"},
};

model::Project Project::Select(const boost::uuids::uuid& id) {
	auto res = _pg->Execute(ClusterHostType::kMaster, kSelect, id);
	if (res.IsEmpty())
		throw errors::NotFound404{};

	return res.AsSingleRow<model::Project>(pg::kRowTag);
}

const pg::Query kInsert{
	"INSERT INTO project.project (space_id, key, name, description, changed_at, sync) "
	"VALUES ($1, $2, $3, $4, $5, $6) RETURNING id",
	pg::Query::Name{"insert_project"},
};

boost::uuids::uuid Project::Insert(
		const boost::uuids::uuid& spaceId,
		const std::string& key,
		const std::string& name,
		const std::string& description,
		std::chrono::system_clock::time_point changedAt,
		SyncDirection::Type sync)
{
	const auto res =_pg->Execute(ClusterHostType::kMaster, kInsert, spaceId, key, name, description, changedAt, sync);
	return res.AsSingleRow<boost::uuids::uuid>();
}

const pg::Query kUpdate {
	"UPDATE project.project SET space_id = $2, key = $3, name = $4, description = $5, changed_at = $6, sync = $7 "
	"WHERE id = $1",
	pg::Query::Name{"update_project"},
};

void Project::Update(const model::Project& project) {
	auto res = _pg->Execute(ClusterHostType::kMaster, kUpdate, project.id, project.spaceId, project.key, project.name, project.description, project.changedAt, project.sync);
	if (!res.RowsAffected())
		throw errors::NotFound404();
}

const pg::Query kDelete {
	"DELETE FROM project.project WHERE id = $1",
	pg::Query::Name{"delete_project"},
};

void Project::Delete(const boost::uuids::uuid& id) {
	auto res = _pg->Execute(ClusterHostType::kMaster, kDelete, id);
	if (!res.RowsAffected())
		throw errors::NotFound404();
}

const pg::Query kSelectProjects{
	"SELECT id, space_id, key, name, description, changed_at, sync FROM project.project "
	"OFFSET $1 LIMIT $2",
	pg::Query::Name{"select_projects"},
};

const pg::Query kCount{
	"SELECT COUNT(*) FROM project.project",
	pg::Query::Name{"count_projects"},
};

PagingResult<model::Project> Project::GetList(int start, int limit) {
	PagingResult<model::Project> data;

	auto trx = _pg->Begin(pg::Transaction::RO);
	auto res = trx.Execute(kSelectProjects, start, limit);
	data.items = res.AsContainer<decltype(data.items)>(pg::kRowTag);
	res = trx.Execute(kCount);
	data.total = res.AsSingleRow<int64_t>();
	trx.Commit();
	return data;
}

} // namespace svetit::project::table