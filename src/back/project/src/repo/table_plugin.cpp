#include "table_plugin.hpp"
#include <shared/errors.hpp>
#include <shared/paging.hpp>

#include <userver/components/component_config.hpp>
#include <userver/components/component_context.hpp>
#include <userver/utils/boost_uuid4.hpp>
#include <userver/yaml_config/merge_schemas.hpp>
#include <userver/storages/postgres/component.hpp>

#include <boost/uuid/uuid_io.hpp>

namespace svetit::project::table {

namespace pg = storages::postgres;
using pg::ClusterHostType;

Plugin::Plugin(pg::ClusterPtr pg)
	: _pg{std::move(pg)}
{}

const pg::Query kSelect{
	"SELECT id, project_id, name, description, key, is_deleted FROM project.plugin WHERE id = $1",
	pg::Query::Name{"select_plugin"},
};

model::Plugin Plugin::Select(int id) {
	auto res = _pg->Execute(ClusterHostType::kMaster, kSelect, id);
	if (res.IsEmpty())
		throw errors::NotFound404{};

	return res.AsSingleRow<model::Plugin>(pg::kRowTag);
}

const pg::Query kInsert{
	"INSERT INTO project.plugin (project_id, name, description, key, is_deleted) "
	"VALUES ($1, $2, $3, $4, $5) RETURNING id",
	pg::Query::Name{"insert_plugin"},
};

int Plugin::Insert(
		const boost::uuids::uuid& projectId,
		const std::string& name,
		const std::string& description,
		const std::string& key,
		bool isDeleted)
{
	const auto res =_pg->Execute(ClusterHostType::kMaster, kInsert, projectId, name, description, key, isDeleted);
	return res.AsSingleRow<int>();
}

const pg::Query kUpdate {
	"UPDATE project.plugin SET project_id = $2, name = $3, description = $4, key = $5, is_deleted = $6 "
	"WHERE id = $1",
	pg::Query::Name{"update_plugin"},
};

void Plugin::Update(const model::Plugin& plugin) {
	auto res = _pg->Execute(ClusterHostType::kMaster, kUpdate, plugin.id, plugin.projectId, plugin.name, plugin.description, plugin.key, plugin.isDeleted);
	if (!res.RowsAffected())
		throw errors::NotFound404();
}

const pg::Query kDelete {
	"DELETE FROM project.plugin WHERE id = $1",
	pg::Query::Name{"delete_plugin"},
};

void Plugin::Delete(int id) {
	auto res = _pg->Execute(ClusterHostType::kMaster, kDelete, id);
	if (!res.RowsAffected())
		throw errors::NotFound404();
}

const pg::Query kSelectSections{
	"SELECT id, project_id, name, description, key, is_deleted FROM project.plugin "
	"OFFSET $1 LIMIT $2",
	pg::Query::Name{"select_plugins"},
};

const pg::Query kCount{
	"SELECT COUNT(*) FROM project.plugin",
	pg::Query::Name{"count_plugins"},
};

PagingResult<model::Plugin> Plugin::GetList(int start, int limit) {
	PagingResult<model::Plugin> data;

	auto trx = _pg->Begin(pg::Transaction::RO);
	auto res = trx.Execute(kSelectSections, start, limit);
	data.items = res.AsContainer<decltype(data.items)>(pg::kRowTag);
	res = trx.Execute(kCount);
	data.total = res.AsSingleRow<int64_t>();
	trx.Commit();
	return data;
}

} // namespace svetit::project::table