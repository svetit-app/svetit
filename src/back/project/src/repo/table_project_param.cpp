#include "table_project_param.hpp"
#include <shared/errors.hpp>
#include <shared/paging.hpp>

#include <userver/components/component_config.hpp>
#include <userver/components/component_context.hpp>
#include <userver/yaml_config/merge_schemas.hpp>
#include <userver/storages/postgres/component.hpp>

namespace svetit::project::table {

namespace pg = storages::postgres;
using pg::ClusterHostType;

ProjectParam::ProjectParam(pg::ClusterPtr pg)
	: _pg{std::move(pg)}
{}

const pg::Query kGet{
	"SELECT project_id, param_id FROM project.project_param WHERE project_id = $1 AND param_id = $2",
	pg::Query::Name{"select_project_param"},
};

model::ProjectParam ProjectParam::Get(const boost::uuids::uuid& projectId, int paramId) {
	auto res = _pg->Execute(ClusterHostType::kMaster, kGet, projectId, paramId);
	if (res.IsEmpty())
		throw errors::NotFound404{};

	return res.AsSingleRow<model::ProjectParam>(pg::kRowTag);
}

const pg::Query kInsert{
	"INSERT INTO project.project_param (project_id, param_id) "
	"VALUES ($1, $2) RETURNING project_id, param_id",
	pg::Query::Name{"insert_project_param"},
};

void ProjectParam::Insert(const boost::uuids::uuid& projectId, int paramId)
{
	_pg->Execute(ClusterHostType::kMaster, kInsert, projectId, paramId);
}

const pg::Query kDelete {
	"DELETE FROM project.project_param WHERE project_id = $1 AND param_id = $2",
	pg::Query::Name{"delete_project_param"},
};

void ProjectParam::Delete(const boost::uuids::uuid& projectId, int paramId) {
	auto res = _pg->Execute(ClusterHostType::kMaster, kDelete, projectId, paramId);
	if (!res.RowsAffected())
		throw errors::NotFound404();
}

const pg::Query kSelectProjectParams{
	"SELECT project_id, param_id FROM project.project_param "
	"OFFSET $1 LIMIT $2",
	pg::Query::Name{"select_project_params"},
};

const pg::Query kCount{
	"SELECT COUNT(*) FROM project.project_param",
	pg::Query::Name{"count_project_params"},
};

PagingResult<model::ProjectParam> ProjectParam::GetList(int start, int limit) {
	PagingResult<model::ProjectParam> data;

	auto trx = _pg->Begin(pg::Transaction::RO);
	auto res = trx.Execute(kSelectProjectParams, start, limit);
	data.items = res.AsContainer<decltype(data.items)>(pg::kRowTag);
	res = trx.Execute(kCount);
	data.total = res.AsSingleRow<int64_t>();
	trx.Commit();
	return data;
}

} // namespace svetit::project::table
