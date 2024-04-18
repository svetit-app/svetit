#include "table_di_plugin_param.hpp"
#include <shared/errors.hpp>
#include <shared/paging.hpp>

#include <userver/components/component_config.hpp>
#include <userver/components/component_context.hpp>
#include <userver/yaml_config/merge_schemas.hpp>
#include <userver/storages/postgres/component.hpp>

namespace svetit::project::table {

namespace pg = storages::postgres;
using pg::ClusterHostType;

DiPluginParam::DiPluginParam(pg::ClusterPtr pg)
	: _pg{std::move(pg)}
{}

const pg::Query kSelect{
	"SELECT di_type_id, param_id FROM project.di_plugin_param WHERE di_type_id = $1 AND param_id = $2",
	pg::Query::Name{"select_di_plugin_param"},
};

model::DiPluginParam DiPluginParam::Select(int diTypeId, int paramId) {
	auto res = _pg->Execute(ClusterHostType::kMaster, kSelect, diTypeId, paramId);
	if (res.IsEmpty())
		throw errors::NotFound404{};

	return res.AsSingleRow<model::DiPluginParam>(pg::kRowTag);
}

const pg::Query kInsert{
	"INSERT INTO project.di_plugin_param (di_type_id, param_id) "
	"VALUES ($1, $2) RETURNING di_type_id, param_id",
	pg::Query::Name{"insert_di_plugin_param"},
};

void DiPluginParam::Insert(int diTypeId, int paramId) {
	_pg->Execute(ClusterHostType::kMaster, kInsert, diTypeId, paramId);
}

const pg::Query kDelete {
	"DELETE FROM project.di_plugin_param WHERE di_type_id = $1 AND param_id = $2",
	pg::Query::Name{"delete_di_plugin_param"},
};

void DiPluginParam::Delete(int diTypeId, int paramId) {
	auto res = _pg->Execute(ClusterHostType::kMaster, kDelete, diTypeId, paramId);
	if (!res.RowsAffected())
		throw errors::NotFound404();
}

const pg::Query kSelectDiPluginParams{
	"SELECT di_type_id, param_id FROM project.di_plugin_param "
	"OFFSET $1 LIMIT $2",
	pg::Query::Name{"select_di_plugin_params"},
};

const pg::Query kCount{
	"SELECT COUNT(*) FROM project.di_plugin_param",
	pg::Query::Name{"count_di_plugin_params"},
};

PagingResult<model::DiPluginParam> DiPluginParam::GetList(int start, int limit) {
	PagingResult<model::DiPluginParam> data;

	auto trx = _pg->Begin(pg::Transaction::RO);
	auto res = trx.Execute(kSelectDiPluginParams, start, limit);
	data.items = res.AsContainer<decltype(data.items)>(pg::kRowTag);
	res = trx.Execute(kCount);
	data.total = res.AsSingleRow<int64_t>();
	trx.Commit();
	return data;
}

} // namespace svetit::project::table