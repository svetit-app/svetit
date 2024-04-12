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

const pg::Query kSelect{
	"SELECT id, project_id, key, name, description FROM project.cc_type WHERE id = $1",
	pg::Query::Name{"select_cc_type"},
};

model::CcType CcType::Select(int id) {
	auto res = _pg->Execute(ClusterHostType::kMaster, kSelect, id);
	if (res.IsEmpty())
		throw errors::NotFound404{};

	return res.AsSingleRow<model::CcType>(pg::kRowTag);
}

const pg::Query kInsert{
	"INSERT INTO project.cc_type (id, project_id, key, name, description) "
	"VALUES ($1, $2, $3, $4, $5) RETURNING id",
	pg::Query::Name{"insert_cc_type"},
};

int CcType::Insert(
		const boost::uuids::uuid& projectId,
		const std::string& key,
		const std::string& name,
		const std::string& description)
{
	const auto res =_pg->Execute(ClusterHostType::kMaster, kInsert, projectId, key, name, description);
	return res.AsSingleRow<int>();
}

const pg::Query kUpdate {
	"UPDATE project.cc_type SET project_id = $2, key = $3, name = $4, description = $5 "
	"WHERE id = $1",
	pg::Query::Name{"update_cc_type"},
};

void CcType::Update(const model::CcType& section) {
	auto res = _pg->Execute(ClusterHostType::kMaster, kUpdate, section.id, section.projectId, section.key, section.name, section.description);
	if (!res.RowsAffected())
		throw errors::NotFound404();
}

const pg::Query kDelete {
	"DELETE FROM project.cc_type WHERE id = $1",
	pg::Query::Name{"delete_cc_type"},
};

void CcType::Delete(int id) {
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