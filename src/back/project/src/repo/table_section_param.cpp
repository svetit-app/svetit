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

const pg::Query kGet{
	"SELECT space_id, section_id, param_id FROM project.section_param WHERE space_id = $1 AND section_id = $2 AND param_id = $3",
	pg::Query::Name{"select_section_param"},
};

model::SectionParam SectionParam::Get(const boost::uuids::uuid& spaceId, int64_t sectionId, int64_t paramId) {
	auto res = _pg->Execute(ClusterHostType::kMaster, kGet, spaceId, sectionId, paramId);
	if (res.IsEmpty())
		throw errors::NotFound404{};

	return res.AsSingleRow<model::SectionParam>(pg::kRowTag);
}

const pg::Query kCreate{
	"INSERT INTO project.section_param (space_id, section_id, param_id) "
	"VALUES ($1, $2, $3)",
	pg::Query::Name{"insert_section_param"},
};

void SectionParam::Create(const model::SectionParam& item)
{
	_pg->Execute(ClusterHostType::kMaster, kCreate, item.spaceId, item.sectionId, item.paramId);
}

void SectionParam::Update(const model::SectionParam& item)
{
	throw errors::Forbidden403();
}

const pg::Query kDelete {
	"DELETE FROM project.section_param WHERE space_id = $1 AND section_id = $2 AND param_id = $3",
	pg::Query::Name{"delete_section_param"},
};

void SectionParam::Delete(const boost::uuids::uuid& spaceId, int64_t sectionId, int64_t paramId) {
	auto res = _pg->Execute(ClusterHostType::kMaster, kDelete, spaceId, sectionId, paramId);
	if (!res.RowsAffected())
		throw errors::NotFound404();
}

const pg::Query kSelectSectionParams{
	"SELECT space_id, section_id, param_id FROM project.section_param "
	"WHERE space_id = $1 AND section_id = $2"
	"OFFSET $3 LIMIT $4",
	pg::Query::Name{"select_section_params"},
};

const pg::Query kCount{
	"SELECT COUNT(*) FROM project.section_param",
	pg::Query::Name{"count_section_params"},
};

PagingResult<model::SectionParam> SectionParam::GetList(const boost::uuids::uuid& spaceId, int64_t sectionId, int start, int limit) {
	PagingResult<model::SectionParam> data;

	auto trx = _pg->Begin(pg::Transaction::RO);
	auto res = trx.Execute(kSelectSectionParams, spaceId, sectionId, start, limit);
	data.items = res.AsContainer<decltype(data.items)>(pg::kRowTag);
	res = trx.Execute(kCount);
	data.total = res.AsSingleRow<int64_t>();
	trx.Commit();
	return data;
}

} // namespace svetit::project::table
