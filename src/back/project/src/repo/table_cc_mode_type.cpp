#include "table_cc_mode_type.hpp"
#include <shared/errors.hpp>
#include <shared/paging.hpp>

#include <userver/components/component_config.hpp>
#include <userver/components/component_context.hpp>
#include <userver/yaml_config/merge_schemas.hpp>
#include <userver/storages/postgres/component.hpp>

namespace svetit::project::table {

namespace pg = storages::postgres;
using pg::ClusterHostType;

CcModeType::CcModeType(pg::ClusterPtr pg)
	: _pg{std::move(pg)}
{}

const pg::Query kSelect{
	"SELECT id, cc_type_id, key, name, is_deleted FROM project.cc_mode_type WHERE id = $1",
	pg::Query::Name{"select_cc_mode_type"}
};

model::CcModeType CcModeType::Select(int id) {
	auto res = _pg->Execute(ClusterHostType::kMaster, kSelect, id);
	if (res.IsEmpty())
		throw errors::NotFound404{};

	return res.AsSingleRow<model::CcModeType>(pg::kRowTag);
}

const pg::Query kInsert{
	"INSERT INTO project.cc_mode_type (cc_type_id, key, name, is_deleted) "
	"VALUES ($1, $2, $3, $4) RETURNING id",
	pg::Query::Name{"insert_cc_mode_type"},
};

int CcModeType::Insert(
	int ccTypeId,
	const std::string& key,
	const std::string& name,
	bool isDeleted)
{
	const auto res =_pg->Execute(ClusterHostType::kMaster, kInsert, ccTypeId, key, name, isDeleted);
	return res.AsSingleRow<int>();
}

const pg::Query kUpdate {
	"UPDATE project.cc_mode_type SET cc_type_id = $2, key = $3, name = $4, is_deleted = $5 "
	"WHERE id = $1",
	pg::Query::Name{"update_cc_mode_type"},
};

void CcModeType::Update(const model::CcModeType& ccModeType) {
	auto res = _pg->Execute(ClusterHostType::kMaster, kUpdate, ccModeType.id, ccModeType.ccTypeId, ccModeType.key, ccModeType.name, ccModeType.isDeleted);
	if (!res.RowsAffected())
		throw errors::NotFound404();
}

const pg::Query kDelete {
	"DELETE FROM project.cc_mode_type WHERE id = $1",
	pg::Query::Name{"delete_cc_mode_type"},
};

void CcModeType::Delete(int id) {
	auto res = _pg->Execute(ClusterHostType::kMaster, kDelete, id);
	if (!res.RowsAffected())
		throw errors::NotFound404();
}

const pg::Query kSelectParamTypes{
	"SELECT id, cc_type_id, key, name, is_deleted FROM project.cc_mode_type "
	"OFFSET $1 LIMIT $2",
	pg::Query::Name{"select_cc_mode_types"},
};

const pg::Query kCount{
	"SELECT COUNT(*) FROM project.cc_mode_type",
	pg::Query::Name{"count_cc_mode_types"},
};

PagingResult<model::CcModeType> CcModeType::GetList(int start, int limit) {
	PagingResult<model::CcModeType> data;

	auto trx = _pg->Begin(pg::Transaction::RO);
	auto res = trx.Execute(kSelectParamTypes, start, limit);
	data.items = res.AsContainer<decltype(data.items)>(pg::kRowTag);
	res = trx.Execute(kCount);
	data.total = res.AsSingleRow<int64_t>();
	trx.Commit();
	return data;
}

} // namespace svetit::project::table