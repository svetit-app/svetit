#include "table_cc_param.hpp"
#include <shared/errors.hpp>
#include <shared/paging.hpp>

#include <userver/components/component_config.hpp>
#include <userver/components/component_context.hpp>
#include <userver/yaml_config/merge_schemas.hpp>
#include <userver/storages/postgres/component.hpp>

namespace svetit::project::table {

namespace pg = storages::postgres;
using pg::ClusterHostType;

CcParam::CcParam(pg::ClusterPtr pg)
	: _pg{std::move(pg)}
{}

const pg::Query kSelect{
	"SELECT cc_id, param_id, is_deleted FROM project.cc_param WHERE cc_id = $1 AND param_id = $2",
	pg::Query::Name{"select_cc_param"},
};

model::CcParam CcParam::Select(int ccId, int paramId) {
	auto res = _pg->Execute(ClusterHostType::kMaster, kSelect, ccId, paramId);
	if (res.IsEmpty())
		throw errors::NotFound404{};

	return res.AsSingleRow<model::CcParam>(pg::kRowTag);
}

const pg::Query kInsert{
	"INSERT INTO project.cc_param (cc_id, param_id, is_deleted) "
	"VALUES ($1, $2, $3) RETURNING cc_id, param_id",
	pg::Query::Name{"insert_cc_param"},
};

void CcParam::Insert(
		int ccId,
		int paramId,
		bool isDeleted)
{
	const auto res =_pg->Execute(ClusterHostType::kMaster, kInsert, ccId, paramId, isDeleted);
	// is needed to return vector or pair with inserted row primary key?
}

const pg::Query kUpdate {
	"UPDATE project.cc_param SET is_deleted = $3 "
	"WHERE cc_id = $1 AND param_id = $2",
	pg::Query::Name{"update_cc_param"},
};

void CcParam::Update(const model::CcParam& ccParam) {
	auto res = _pg->Execute(ClusterHostType::kMaster, kUpdate, ccParam.ccId, ccParam.paramId, ccParam.isDeleted);
	if (!res.RowsAffected())
		throw errors::NotFound404();
}

const pg::Query kDelete {
	"DELETE FROM project.cc_param WHERE cc_id = $1 AND param_id = $2",
	pg::Query::Name{"delete_cc_param"},
};

void CcParam::Delete(int ccId, int paramId) {
	auto res = _pg->Execute(ClusterHostType::kMaster, kDelete, ccId, paramId);
	if (!res.RowsAffected())
		throw errors::NotFound404();
}

const pg::Query kSelectCcIds{
	"SELECT cc_id, param_id, is_deleted FROM project.cc_param "
	"OFFSET $1 LIMIT $2",
	pg::Query::Name{"select_cc_params"},
};

const pg::Query kCount{
	"SELECT COUNT(*) FROM project.cc_param",
	pg::Query::Name{"count_cc_params"},
};

PagingResult<model::CcParam> CcParam::GetList(int start, int limit) {
	PagingResult<model::CcParam> data;

	auto trx = _pg->Begin(pg::Transaction::RO);
	auto res = trx.Execute(kSelectCcIds, start, limit);
	data.items = res.AsContainer<decltype(data.items)>(pg::kRowTag);
	res = trx.Execute(kCount);
	data.total = res.AsSingleRow<int64_t>();
	trx.Commit();
	return data;
}

} // namespace svetit::project::table