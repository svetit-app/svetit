#include "table_plugin.hpp"
#include <shared/errors.hpp>
#include <shared/paging.hpp>

#include <userver/components/component_config.hpp>
#include <userver/components/component_context.hpp>
#include <userver/yaml_config/merge_schemas.hpp>
#include <userver/storages/postgres/component.hpp>

namespace svetit::project::table {

namespace pg = storages::postgres;
using pg::ClusterHostType;

Plugin::Plugin(pg::ClusterPtr pg)
	: _pg{std::move(pg)}
{}

const pg::Query kGet{
	"SELECT id, space_id, project_id, name, description, key FROM project.plugin WHERE id = $1 AND space_id = $2",
	pg::Query::Name{"select_plugin"},
};

model::Plugin Plugin::Get(const boost::uuids::uuid& spaceId, int64_t id) {
	auto res = _pg->Execute(ClusterHostType::kMaster, kGet, id, spaceId);
	if (res.IsEmpty())
		throw errors::NotFound404{};

	return res.AsSingleRow<model::Plugin>(pg::kRowTag);
}

const pg::Query kCreate{
	"INSERT INTO project.plugin (space_id, project_id, name, description, key) "
	"VALUES ($1, $2, $3, $4, $5)"
	"RETURNING id",
	pg::Query::Name{"insert_plugin"},
};

int64_t Plugin::Create(const model::Plugin& plugin)
{
	auto res = _pg->Execute(ClusterHostType::kMaster, kCreate, plugin.spaceId, plugin.projectId, plugin.name, plugin.description, plugin.key);
	return res.AsSingleRow<int64_t>();
}

const pg::Query kUpdate {
	"UPDATE project.plugin SET project_id = $3, name = $4, description = $5, key = $6 "
	"WHERE id = $1 AND space_id = $2",
	pg::Query::Name{"update_plugin"},
};

void Plugin::Update(const model::Plugin& plugin) {
	auto res = _pg->Execute(ClusterHostType::kMaster, kUpdate, plugin.id, plugin.spaceId, plugin.projectId, plugin.name, plugin.description, plugin.key);
	if (!res.RowsAffected())
		throw errors::NotFound404();
}

const pg::Query kDelete {
	"DELETE FROM project.plugin WHERE id = $1 AND space_id = $2",
	pg::Query::Name{"delete_plugin"},
};

void Plugin::Delete(const boost::uuids::uuid& spaceId, int64_t id) {
	auto res = _pg->Execute(ClusterHostType::kMaster, kDelete, id, spaceId);
	if (!res.RowsAffected())
		throw errors::NotFound404();
}

const pg::Query kSelectPlugins{
	"SELECT id, space_id, project_id, name, description, key FROM project.plugin "
	"WHERE space_id = $1 AND project_id = $2"
	"OFFSET $3 LIMIT $4",
	pg::Query::Name{"select_plugins"},
};

const pg::Query kCount{
	"SELECT COUNT(*) FROM project.plugin",
	pg::Query::Name{"count_plugins"},
};

PagingResult<model::Plugin> Plugin::GetList(const boost::uuids::uuid& spaceId, const boost::uuids::uuid& projectId, int start, int limit) {
	PagingResult<model::Plugin> data;

	auto trx = _pg->Begin(pg::Transaction::RO);
	auto res = trx.Execute(kSelectPlugins, spaceId, projectId, start, limit);
	data.items = res.AsContainer<decltype(data.items)>(pg::kRowTag);
	res = trx.Execute(kCount);
	data.total = res.AsSingleRow<int64_t>();
	trx.Commit();
	return data;
}

} // namespace svetit::project::table
