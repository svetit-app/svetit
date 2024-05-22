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
	"SELECT cc_type_id, di_type_id FROM project.cc_type_di_type WHERE cc_type_id = $1 AND di_type_id = $2",
	pg::Query::Name{"select_cc_type_di_type"},
};

model::CcTypeDiType CcTypeDiType::Get(const boost::uuids::uuid& spaceId, int64_t ccTypeId, int64_t diTypeId) {
	auto res = _pg->Execute(ClusterHostType::kMaster, kGet, ccTypeId, diTypeId);
	if (res.IsEmpty())
		throw errors::NotFound404{};

	return res.AsSingleRow<model::CcTypeDiType>(pg::kRowTag);
}

const pg::Query kInsert{
	"INSERT INTO project.cc_type_di_type (cc_type_id, di_type_id) "
	"VALUES ($1, $2) RETURNING cc_type_id, di_type_id",
	pg::Query::Name{"insert_cc_type_di_type"},
};

void CcTypeDiType::Insert(int64_t ccTypeId, int64_t diTypeId) {
	_pg->Execute(ClusterHostType::kMaster, kInsert, ccTypeId, diTypeId);
}

const pg::Query kDelete {
	"DELETE FROM project.cc_type_di_type WHERE cc_type_id = $1 AND di_type_id = $2",
	pg::Query::Name{"delete_cc_type_di_type"},
};

void CcTypeDiType::Delete(const boost::uuids::uuid& spaceId, int64_t ccTypeId, int64_t diTypeId) {
	auto res = _pg->Execute(ClusterHostType::kMaster, kDelete, ccTypeId, diTypeId);
	if (!res.RowsAffected())
		throw errors::NotFound404();
}

const pg::Query kSelectCcTypeDiTypes{
	"SELECT cc_type_id, di_type_id FROM project.cc_type_di_type "
	"OFFSET $1 LIMIT $2",
	pg::Query::Name{"select_cc_type_di_types"},
};

const pg::Query kCount{
	"SELECT COUNT(*) FROM project.cc_type_di_type",
	pg::Query::Name{"count_cc_type_di_types"},
};

PagingResult<model::CcTypeDiType> CcTypeDiType::GetList(const boost::uuids::uuid& spaceId, int start, int limit) {
	PagingResult<model::CcTypeDiType> data;

	auto trx = _pg->Begin(pg::Transaction::RO);
	auto res = trx.Execute(kSelectCcTypeDiTypes, start, limit);
	data.items = res.AsContainer<decltype(data.items)>(pg::kRowTag);
	res = trx.Execute(kCount);
	data.total = res.AsSingleRow<int64_t>();
	trx.Commit();
	return data;
}

} // namespace svetit::project::table
