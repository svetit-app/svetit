#include "table_cc_status_category.hpp"
#include <shared/errors.hpp>
#include <shared/paging.hpp>

#include <userver/components/component_config.hpp>
#include <userver/components/component_context.hpp>
#include <userver/yaml_config/merge_schemas.hpp>
#include <userver/storages/postgres/component.hpp>

namespace svetit::project::table {

namespace pg = storages::postgres;
using pg::ClusterHostType;

CcStatusCategory::CcStatusCategory(pg::ClusterPtr pg)
	: _pg{std::move(pg)}
{}

const pg::Query kGet{
	"SELECT id, project_id, key, name, color FROM project.cc_status_category WHERE id = $1",
	pg::Query::Name{"select_cc_status_category"},
};

model::CcStatusCategory CcStatusCategory::Get(int id) {
	auto res = _pg->Execute(ClusterHostType::kMaster, kGet, id);
	if (res.IsEmpty())
		throw errors::NotFound404{};

	return res.AsSingleRow<model::CcStatusCategory>(pg::kRowTag);
}

const pg::Query kInsert{
	"INSERT INTO project.cc_status_category (project_id, key, name, color) "
	"VALUES ($1, $2, $3, $4)"
	"RETURNING id",
	pg::Query::Name{"insert_cc_status_category"},
};

int64_t CcStatusCategory::Create(const model::CcStatusCategory& ccStatusCategory)
{
	auto res =_pg->Execute(ClusterHostType::kMaster, kInsert, ccStatusCategory.projectId, ccStatusCategory.key, ccStatusCategory.name, ccStatusCategory.color);
	return res.AsSingleRow<int64_t>();
}

const pg::Query kUpdate {
	"UPDATE project.cc_status_category SET project_id = $2, key = $3, name = $4, color = $5 "
	"WHERE id = $1",
	pg::Query::Name{"update_cc_status_category"},
};

void CcStatusCategory::Update(const model::CcStatusCategory& ccStatusCategory) {
	auto res = _pg->Execute(ClusterHostType::kMaster, kUpdate, ccStatusCategory.id, ccStatusCategory.projectId, ccStatusCategory.key, ccStatusCategory.name, ccStatusCategory.color);
	if (!res.RowsAffected())
		throw errors::NotFound404();
}

const pg::Query kDelete {
	"DELETE FROM project.cc_status_category WHERE id = $1",
	pg::Query::Name{"delete_cc_status_category"},
};

void CcStatusCategory::Delete(int id) {
	auto res = _pg->Execute(ClusterHostType::kMaster, kDelete, id);
	if (!res.RowsAffected())
		throw errors::NotFound404();
}

const pg::Query kSelectCcStatusCategories{
	"SELECT id, project_id, key, name, color FROM project.cc_status_category "
	"OFFSET $1 LIMIT $2",
	pg::Query::Name{"select_cc_status_categories"},
};

const pg::Query kCount{
	"SELECT COUNT(*) FROM project.cc_status_category",
	pg::Query::Name{"count_cc_status_categories"},
};

PagingResult<model::CcStatusCategory> CcStatusCategory::GetList(int start, int limit) {
	PagingResult<model::CcStatusCategory> data;

	auto trx = _pg->Begin(pg::Transaction::RO);
	auto res = trx.Execute(kSelectCcStatusCategories, start, limit);
	data.items = res.AsContainer<decltype(data.items)>(pg::kRowTag);
	res = trx.Execute(kCount);
	data.total = res.AsSingleRow<int64_t>();
	trx.Commit();
	return data;
}

} // namespace svetit::project::table
