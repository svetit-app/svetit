#include "table_cc_type_di_type.hpp"
#include <shared/errors.hpp>
#include <shared/paging.hpp>

#include <userver/components/component_config.hpp>
#include <userver/components/component_context.hpp>
#include <userver/yaml_config/merge_schemas.hpp>
#include <userver/storages/postgres/component.hpp>

namespace svetit::project::table {

namespace pg = storages::postgres;
using pg::ClusterHostType;

CcTypeDiType::CcTypeDiType(pg::ClusterPtr pg)
	: _pg{std::move(pg)}
{}

const pg::Query kGet{
	"SELECT space_id, cc_type_id, di_type_id FROM project.cc_type_di_type WHERE space_id = $1 AND cc_type_id = $2 AND di_type_id = $3",
	pg::Query::Name{"select_cc_type_di_type"},
};

model::CcTypeDiType CcTypeDiType::Get(const boost::uuids::uuid& spaceId, int64_t ccTypeId, int64_t diTypeId) {
	auto res = _pg->Execute(ClusterHostType::kMaster, kGet, spaceId, ccTypeId, diTypeId);
	if (res.IsEmpty())
		throw errors::NotFound404{};

	return res.AsSingleRow<model::CcTypeDiType>(pg::kRowTag);
}

const pg::Query kCreate{
	"INSERT INTO project.cc_type_di_type (space_id, cc_type_id, di_type_id) "
	"VALUES ($1, $2, $3)",
	pg::Query::Name{"insert_cc_type_di_type"},
};

void CcTypeDiType::Create(const model::CcTypeDiType& item) {
	_pg->Execute(ClusterHostType::kMaster, kCreate, item.spaceId, item.ccTypeId, item.diTypeId);
}

void CcTypeDiType::Update(const model::CcTypeDiType&) {
	throw errors::Forbidden403();
}

const pg::Query kDelete {
	"DELETE FROM project.cc_type_di_type WHERE space_id = $1 AND cc_type_id = $2 AND di_type_id = $3",
	pg::Query::Name{"delete_cc_type_di_type"},
};

void CcTypeDiType::Delete(const boost::uuids::uuid& spaceId, int64_t ccTypeId, int64_t diTypeId) {
	auto res = _pg->Execute(ClusterHostType::kMaster, kDelete, spaceId, ccTypeId, diTypeId);
	if (!res.RowsAffected())
		throw errors::NotFound404();
}

const pg::Query kSelectCcTypeDiTypes{
	"SELECT space_id, cc_type_id, di_type_id FROM project.cc_type_di_type "
	"WHERE space_id = $1 AND cc_type_id = $2"
	"OFFSET $3 LIMIT $4",
	pg::Query::Name{"select_cc_type_di_types"},
};

const pg::Query kCount{
	"SELECT COUNT(*) FROM project.cc_type_di_type",
	pg::Query::Name{"count_cc_type_di_types"},
};

PagingResult<model::CcTypeDiType> CcTypeDiType::GetList(const boost::uuids::uuid& spaceId, int64_t ccTypeId, int start, int limit) {
	PagingResult<model::CcTypeDiType> data;

	auto trx = _pg->Begin(pg::Transaction::RO);
	auto res = trx.Execute(kSelectCcTypeDiTypes, spaceId, ccTypeId, start, limit);
	data.items = res.AsContainer<decltype(data.items)>(pg::kRowTag);
	res = trx.Execute(kCount);
	data.total = res.AsSingleRow<int64_t>();
	trx.Commit();
	return data;
}

} // namespace svetit::project::table
