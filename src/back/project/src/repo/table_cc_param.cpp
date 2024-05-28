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

const pg::Query kGet{
	"SELECT space_id, cc_id, param_id FROM project.cc_param WHERE space_id = $1 AND cc_id = $2 AND param_id = $3",
	pg::Query::Name{"select_cc_param"},
};

model::CcParam CcParam::Get(const boost::uuids::uuid& spaceId, int64_t ccId, int64_t paramId) {
	auto res = _pg->Execute(ClusterHostType::kMaster, kGet, spaceId, ccId, paramId);
	if (res.IsEmpty())
		throw errors::NotFound404{};

	return res.AsSingleRow<model::CcParam>(pg::kRowTag);
}

const pg::Query kCreate{
	"INSERT INTO project.cc_param (space_id, cc_id, param_id) "
	"VALUES ($1, $2, $3)",
	pg::Query::Name{"insert_cc_param"},
};

void CcParam::Create(const model::CcParam& item) {
	_pg->Execute(ClusterHostType::kMaster, kCreate, item.spaceId, item.ccId, item.paramId);
}

void CcParam::Update(const model::CcParam&) {
	throw errors::Forbidden403();
}

const pg::Query kDelete {
	"DELETE FROM project.cc_param WHERE space_id = $1 AND cc_id = $2 AND param_id = $3",
	pg::Query::Name{"delete_cc_param"},
};

void CcParam::Delete(const boost::uuids::uuid& spaceId, int64_t ccId, int64_t paramId) {
	auto res = _pg->Execute(ClusterHostType::kMaster, kDelete, spaceId, ccId, paramId);
	if (!res.RowsAffected())
		throw errors::NotFound404();
}

const pg::Query kSelectCcIds{
	"SELECT space_id, cc_id, param_id FROM project.cc_param "
	"WHERE space_id = $1 AND cc_id = $2"
	"OFFSET $3 LIMIT $4",
	pg::Query::Name{"select_cc_params"},
};

const pg::Query kCount{
	"SELECT COUNT(*) FROM project.cc_param "
	"WHERE space_id = $1 AND cc_id = $2",
	pg::Query::Name{"count_cc_params"},
};

PagingResult<model::CcParam> CcParam::GetList(const boost::uuids::uuid& spaceId, int64_t ccId, int start, int limit) {
	PagingResult<model::CcParam> data;

	auto trx = _pg->Begin(pg::Transaction::RO);
	auto res = trx.Execute(kSelectCcIds, spaceId, ccId, start, limit);
	data.items = res.AsContainer<decltype(data.items)>(pg::kRowTag);
	res = trx.Execute(kCount, spaceId, ccId);
	data.total = res.AsSingleRow<int64_t>();
	trx.Commit();
	return data;
}

} // namespace svetit::project::table
