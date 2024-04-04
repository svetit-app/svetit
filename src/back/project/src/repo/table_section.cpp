#include "table_section.hpp"
#include <shared/errors.hpp>
#include <shared/paging.hpp>

#include <userver/components/component_config.hpp>
#include <userver/components/component_context.hpp>
#include <userver/yaml_config/merge_schemas.hpp>
#include <userver/storages/postgres/component.hpp>

namespace svetit::project::table {

namespace pg = storages::postgres;
using pg::ClusterHostType;

Section::Section(pg::ClusterPtr pg)
	: _pg{std::move(pg)}
{}

const pg::Query kSelect{
	"SELECT id, project_id, name, is_deleted FROM project.section WHERE id = $1",
	pg::Query::Name{"select_section"},
};

model::Section Section::Select(int id) {
	auto res = _pg->Execute(ClusterHostType::kMaster, kSelect, id);
	if (res.IsEmpty())
		throw errors::NotFound404{};

	return res.AsSingleRow<model::Section>(pg::kRowTag);
}

const pg::Query kInsert{
	"INSERT INTO project.section (project_id, name, is_deleted) "
	"VALUES ($1, $2, $3) RETURNING id",
	pg::Query::Name{"insert_section"},
};

int Section::Insert(
		const boost::uuids::uuid& projectId,
		const std::string& name,
		bool isDeleted)
{
	const auto res =_pg->Execute(ClusterHostType::kMaster, kInsert, projectId, name, isDeleted);
	return res.AsSingleRow<int>();
}

const pg::Query kUpdate {
	"UPDATE project.section SET project_id = $2, name = $3, is_deleted = $4 "
	"WHERE id = $1",
	pg::Query::Name{"update_section"},
};

void Section::Update(const model::Section& section) {
	auto res = _pg->Execute(ClusterHostType::kMaster, kUpdate, section.id, section.projectId, section.name, section.isDeleted);
	if (!res.RowsAffected())
		throw errors::NotFound404();
}

const pg::Query kDelete {
	"DELETE FROM project.section WHERE id = $1",
	pg::Query::Name{"delete_section"},
};

void Section::Delete(int id) {
	auto res = _pg->Execute(ClusterHostType::kMaster, kDelete, id);
	if (!res.RowsAffected())
		throw errors::NotFound404();
}

const pg::Query kSelectSections{
	"SELECT id, project_id, name, is_deleted FROM project.section "
	"OFFSET $1 LIMIT $2",
	pg::Query::Name{"select_sections"},
};

const pg::Query kCount{
	"SELECT COUNT(*) FROM project.section",
	pg::Query::Name{"count_sections"},
};

PagingResult<model::Section> Section::GetList(int start, int limit) {
	PagingResult<model::Section> data;

	auto trx = _pg->Begin(pg::Transaction::RO);
	auto res = trx.Execute(kSelectSections, start, limit);
	data.items = res.AsContainer<decltype(data.items)>(pg::kRowTag);
	res = trx.Execute(kCount);
	data.total = res.AsSingleRow<int64_t>();
	trx.Commit();
	return data;
}

} // namespace svetit::project::table