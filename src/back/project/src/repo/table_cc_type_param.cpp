#include "table_cc_type_param.hpp"
#include <shared/errors.hpp>
#include <shared/paging.hpp>

#include <userver/components/component_config.hpp>
#include <userver/components/component_context.hpp>
#include <userver/yaml_config/merge_schemas.hpp>
#include <userver/storages/postgres/component.hpp>

namespace svetit::project::table {

namespace pg = storages::postgres;
using pg::ClusterHostType;

CcTypeParam::CcTypeParam(pg::ClusterPtr pg)
	: _pg{std::move(pg)}
{}

const pg::Query kSelect{
	"SELECT cc_type_id, param_id FROM project.cc_type_param WHERE cc_type_id = $1 AND param_id = $2",
	pg::Query::Name{"select_cc_type_param"},
};

model::CcTypeParam CcTypeParam::Select(int ccTypeId, int paramId) {
	auto res = _pg->Execute(ClusterHostType::kMaster, kSelect, ccTypeId, paramId);
	if (res.IsEmpty())
		throw errors::NotFound404{};

	return res.AsSingleRow<model::CcTypeParam>(pg::kRowTag);
}

const pg::Query kInsert{
	"INSERT INTO project.cc_type_param (cc_type_id, param_id) "
	"VALUES ($1, $2) RETURNING cc_type_id, param_id",
	pg::Query::Name{"insert_cc_type_param"},
};

void CcTypeParam::Insert(int ccTypeId, int paramId)
{
	const auto res =_pg->Execute(ClusterHostType::kMaster, kInsert, ccTypeId, paramId);
	// is needed to return vector or pair with inserted row primary key?
}

const pg::Query kDelete {
	"DELETE FROM project.cc_type_param WHERE cc_type_id = $1 AND param_id = $2",
	pg::Query::Name{"delete_cc_type_param"},
};

void CcTypeParam::Delete(int ccTypeId, int paramId) {
	auto res = _pg->Execute(ClusterHostType::kMaster, kDelete, ccTypeId, paramId);
	if (!res.RowsAffected())
		throw errors::NotFound404();
}

const pg::Query kSelectCcTypeParams{
	"SELECT cc_type_id, param_id FROM project.cc_type_param "
	"OFFSET $1 LIMIT $2",
	pg::Query::Name{"select_cc_type_params"},
};

const pg::Query kCount{
	"SELECT COUNT(*) FROM project.cc_type_param",
	pg::Query::Name{"count_cc_type_params"},
};

PagingResult<model::CcTypeParam> CcTypeParam::GetList(int start, int limit) {
	PagingResult<model::CcTypeParam> data;

	auto trx = _pg->Begin(pg::Transaction::RO);
	auto res = trx.Execute(kSelectCcTypeParams, start, limit);
	data.items = res.AsContainer<decltype(data.items)>(pg::kRowTag);
	res = trx.Execute(kCount);
	data.total = res.AsSingleRow<int64_t>();
	trx.Commit();
	return data;
}

} // namespace svetit::project::table