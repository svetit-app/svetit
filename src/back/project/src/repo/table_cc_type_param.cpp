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

const pg::Query kGet{
	"SELECT space_id, cc_type_id, param_id FROM project.cc_type_param WHERE space_id = $1 AND cc_type_id = $2 AND param_id = $3",
	pg::Query::Name{"select_cc_type_param"},
};

model::CcTypeParam CcTypeParam::Get(const boost::uuids::uuid& spaceId, int64_t ccTypeId, int64_t paramId) {
	auto res = _pg->Execute(ClusterHostType::kMaster, kGet, spaceId, ccTypeId, paramId);
	if (res.IsEmpty())
		throw errors::NotFound404{};

	return res.AsSingleRow<model::CcTypeParam>(pg::kRowTag);
}

const pg::Query kCreate{
	"INSERT INTO project.cc_type_param (space_id, cc_type_id, param_id) "
	"VALUES ($1, $2, $3)",
	pg::Query::Name{"insert_cc_type_param"},
};

void CcTypeParam::Create(const model::CcTypeParam& item)
{
	_pg->Execute(ClusterHostType::kMaster, kCreate, item.spaceId, item.ccTypeId, item.paramId);
}

void CcTypeParam::Update(const model::CcTypeParam&)
{
	throw errors::Forbidden403();
}

const pg::Query kDelete {
	"DELETE FROM project.cc_type_param WHERE space_id = $1 AND cc_type_id = $2 AND param_id = $3",
	pg::Query::Name{"delete_cc_type_param"},
};

void CcTypeParam::Delete(const boost::uuids::uuid& spaceId, int64_t ccTypeId, int64_t paramId) {
	auto res = _pg->Execute(ClusterHostType::kMaster, kDelete, spaceId, ccTypeId, paramId);
	if (!res.RowsAffected())
		throw errors::NotFound404();
}

const pg::Query kSelectCcTypeParams{
	"SELECT space_id, cc_type_id, param_id FROM project.cc_type_param "
	"WHERE space_id = $1 AND cc_type_id = $2"
	"OFFSET $3 LIMIT $4",
	pg::Query::Name{"select_cc_type_params"},
};

const pg::Query kCount{
	"SELECT COUNT(*) FROM project.cc_type_param",
	pg::Query::Name{"count_cc_type_params"},
};

PagingResult<model::CcTypeParam> CcTypeParam::GetList(const boost::uuids::uuid& spaceId, int64_t ccTypeId, int start, int limit) {
	PagingResult<model::CcTypeParam> data;

	auto trx = _pg->Begin(pg::Transaction::RO);
	auto res = trx.Execute(kSelectCcTypeParams, spaceId, ccTypeId, start, limit);
	data.items = res.AsContainer<decltype(data.items)>(pg::kRowTag);
	res = trx.Execute(kCount);
	data.total = res.AsSingleRow<int64_t>();
	trx.Commit();
	return data;
}

} // namespace svetit::project::table
