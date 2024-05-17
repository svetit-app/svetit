#include "table_cc_type.hpp"
#include <shared/errors.hpp>
#include <shared/paging.hpp>

#include <userver/components/component_config.hpp>
#include <userver/components/component_context.hpp>
#include <userver/yaml_config/merge_schemas.hpp>
#include <userver/storages/postgres/component.hpp>

namespace svetit::project::table {

namespace pg = storages::postgres;
using pg::ClusterHostType;

CcType::CcType(pg::ClusterPtr pg)
	: _pg{std::move(pg)}
{}

const pg::Query kGet{
	"SELECT id, project_id, key, name, description FROM project.cc_type WHERE id = $1",
	pg::Query::Name{"select_cc_type"},
};

model::CcType CcType::Get(int64_t id) {
	auto res = _pg->Execute(ClusterHostType::kMaster, kGet, id);
	if (res.IsEmpty())
		throw errors::NotFound404{};

	return res.AsSingleRow<model::CcType>(pg::kRowTag);
}

const pg::Query kCreate{
	"INSERT INTO project.cc_type (project_id, key, name, description) "
	"VALUES ($1, $2, $3, $4)"
	"RETURNING id",
	pg::Query::Name{"insert_cc_type"},
};

int64_t CcType::Create(const model::CcType& ccType)
{
	auto res =_pg->Execute(ClusterHostType::kMaster, kCreate, ccType.projectId, ccType.key, ccType.name, ccType.description);
	return res.AsSingleRow<int64_t>();
}

const pg::Query kUpdate {
	"UPDATE project.cc_type SET project_id = $2, key = $3, name = $4, description = $5 "
	"WHERE id = $1",
	pg::Query::Name{"update_cc_type"},
};

void CcType::Update(const model::CcType& ccType) {
	auto res = _pg->Execute(ClusterHostType::kMaster, kUpdate, ccType.id, ccType.projectId, ccType.key, ccType.name, ccType.description);
	if (!res.RowsAffected())
		throw errors::NotFound404();
}

const pg::Query kDelete {
	"DELETE FROM project.cc_type WHERE id = $1",
	pg::Query::Name{"delete_cc_type"},
};

void CcType::Delete(int64_t id) {
	auto res = _pg->Execute(ClusterHostType::kMaster, kDelete, id);
	if (!res.RowsAffected())
		throw errors::NotFound404();
}

const pg::Query kSelectCcTypes{
	"SELECT id, project_id, key, name, description FROM project.cc_type "
	"OFFSET $1 LIMIT $2",
	pg::Query::Name{"select_cc_types"},
};

const pg::Query kCount{
	"SELECT COUNT(*) FROM project.cc_type",
	pg::Query::Name{"count_cc_types"},
};

PagingResult<model::CcType> CcType::GetList(int start, int limit) {
	PagingResult<model::CcType> data;

	auto trx = _pg->Begin(pg::Transaction::RO);
	auto res = trx.Execute(kSelectCcTypes, start, limit);
	data.items = res.AsContainer<decltype(data.items)>(pg::kRowTag);
	res = trx.Execute(kCount);
	data.total = res.AsSingleRow<int64_t>();
	trx.Commit();
	return data;
}

} // namespace svetit::project::table
