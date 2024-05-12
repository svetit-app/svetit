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

const pg::Query kGet{
	"SELECT id, project_id, name FROM project.section WHERE id = $1",
	pg::Query::Name{"get_section"},
};

model::Section Section::Get(int64_t id) {
	auto res = _pg->Execute(ClusterHostType::kMaster, kGet, id);
	if (res.IsEmpty())
		throw errors::NotFound404{};

	return res.AsSingleRow<model::Section>(pg::kRowTag);
}

const pg::Query kCreate{
	"INSERT INTO project.section (project_id, name) "
	"VALUES ($1, $2)"
	"RETURNING id",
	pg::Query::Name{"create_section"},
};

int64_t Section::Create(const model::Section& item)
{
	auto res = _pg->Execute(ClusterHostType::kMaster, kCreate, item.projectId, item.name);
	return res.AsSingleRow<int64_t>();
}

const pg::Query kUpdate {
	"UPDATE project.section SET project_id = $2, name = $3 "
	"WHERE id = $1",
	pg::Query::Name{"update_section"},
};

void Section::Update(const model::Section& section) {
	auto res = _pg->Execute(ClusterHostType::kMaster, kUpdate, section.id, section.projectId, section.name);
	if (!res.RowsAffected())
		throw errors::NotFound404();
}

const pg::Query kDelete {
	"DELETE FROM project.section WHERE id = $1",
	pg::Query::Name{"delete_section"},
};

void Section::Delete(int64_t id) {
	auto res = _pg->Execute(ClusterHostType::kMaster, kDelete, id);
	if (!res.RowsAffected())
		throw errors::NotFound404();
}

const pg::Query kSelectSections{
	"SELECT id, project_id, name FROM project.section "
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
