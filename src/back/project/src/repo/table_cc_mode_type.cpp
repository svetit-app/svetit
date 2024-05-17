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

const pg::Query kGet{
	"SELECT id, cc_type_id, key, name FROM project.cc_mode_type WHERE id = $1",
	pg::Query::Name{"select_cc_mode_type"}
};

model::CcModeType CcModeType::Get(int64_t id) {
	auto res = _pg->Execute(ClusterHostType::kMaster, kGet, id);
	if (res.IsEmpty())
		throw errors::NotFound404{};

	return res.AsSingleRow<model::CcModeType>(pg::kRowTag);
}

const pg::Query kCreate{
	"INSERT INTO project.cc_mode_type (cc_type_id, key, name) "
	"VALUES ($1, $2, $3)"
	"RETURNING id",
	pg::Query::Name{"insert_cc_mode_type"},
};

int64_t CcModeType::Create(const model::CcModeType& ccModeType)
{
	auto res = _pg->Execute(ClusterHostType::kMaster, kCreate, ccModeType.ccTypeId, ccModeType.key, ccModeType.name);
	return res.AsSingleRow<int64_t>();
}

const pg::Query kUpdate {
	"UPDATE project.cc_mode_type SET cc_type_id = $2, key = $3, name = $4 "
	"WHERE id = $1",
	pg::Query::Name{"update_cc_mode_type"},
};

void CcModeType::Update(const model::CcModeType& ccModeType) {
	auto res = _pg->Execute(ClusterHostType::kMaster, kUpdate, ccModeType.id, ccModeType.ccTypeId, ccModeType.key, ccModeType.name);
	if (!res.RowsAffected())
		throw errors::NotFound404();
}

const pg::Query kDelete {
	"DELETE FROM project.cc_mode_type WHERE id = $1",
	pg::Query::Name{"delete_cc_mode_type"},
};

void CcModeType::Delete(int64_t id) {
	auto res = _pg->Execute(ClusterHostType::kMaster, kDelete, id);
	if (!res.RowsAffected())
		throw errors::NotFound404();
}

const pg::Query kSelectCcModeTypes{
	"SELECT id, cc_type_id, key, name FROM project.cc_mode_type "
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
	auto res = trx.Execute(kSelectCcModeTypes, start, limit);
	data.items = res.AsContainer<decltype(data.items)>(pg::kRowTag);
	res = trx.Execute(kCount);
	data.total = res.AsSingleRow<int64_t>();
	trx.Commit();
	return data;
}

} // namespace svetit::project::table
