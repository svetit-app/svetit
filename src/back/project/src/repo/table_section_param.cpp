#include "table_section_param.hpp"
#include <shared/errors.hpp>
#include <shared/paging.hpp>

#include <userver/components/component_config.hpp>
#include <userver/components/component_context.hpp>
#include <userver/yaml_config/merge_schemas.hpp>
#include <userver/storages/postgres/component.hpp>

namespace svetit::project::table {

namespace pg = storages::postgres;
using pg::ClusterHostType;

SectionParam::SectionParam(pg::ClusterPtr pg)
	: _pg{std::move(pg)}
{}

const pg::Query kSelect{
	"SELECT section_id, param_id FROM project.section_param WHERE section_id = $1 AND param_id = $2",
	pg::Query::Name{"select_section_param"},
};

model::SectionParam SectionParam::Select(int sectionId, int paramId) {
	auto res = _pg->Execute(ClusterHostType::kMaster, kSelect, sectionId, paramId);
	if (res.IsEmpty())
		throw errors::NotFound404{};

	return res.AsSingleRow<model::SectionParam>(pg::kRowTag);
}

const pg::Query kInsert{
	"INSERT INTO project.section_param (section_id, param_id) "
	"VALUES ($1, $2) RETURNING section_id, param_id",
	pg::Query::Name{"insert_section_param"},
};

void SectionParam::Insert(int sectionId, int paramId)
{
	_pg->Execute(ClusterHostType::kMaster, kInsert, sectionId, paramId);
}

const pg::Query kDelete {
	"DELETE FROM project.section_param WHERE section_id = $1 AND param_id = $2",
	pg::Query::Name{"delete_section_param"},
};

void SectionParam::Delete(int sectionId, int paramId) {
	auto res = _pg->Execute(ClusterHostType::kMaster, kDelete, sectionId, paramId);
	if (!res.RowsAffected())
		throw errors::NotFound404();
}

const pg::Query kSelectSectionParams{
	"SELECT section_id, param_id FROM project.section_param "
	"OFFSET $1 LIMIT $2",
	pg::Query::Name{"select_section_params"},
};

const pg::Query kCount{
	"SELECT COUNT(*) FROM project.section_param",
	pg::Query::Name{"count_section_params"},
};

PagingResult<model::SectionParam> SectionParam::GetList(int start, int limit) {
	PagingResult<model::SectionParam> data;

	auto trx = _pg->Begin(pg::Transaction::RO);
	auto res = trx.Execute(kSelectSectionParams, start, limit);
	data.items = res.AsContainer<decltype(data.items)>(pg::kRowTag);
	res = trx.Execute(kCount);
	data.total = res.AsSingleRow<int64_t>();
	trx.Commit();
	return data;
}

} // namespace svetit::project::table