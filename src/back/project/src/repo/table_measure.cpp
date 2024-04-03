#include "table_measure.hpp"
#include <shared/errors.hpp>
#include <shared/paging.hpp>

#include <userver/components/component_config.hpp>
#include <userver/components/component_context.hpp>
#include <userver/utils/boost_uuid4.hpp>
#include <userver/yaml_config/merge_schemas.hpp>
#include <userver/storages/postgres/component.hpp>

#include <boost/uuid/uuid_io.hpp>

namespace svetit::project::table {

namespace pg = storages::postgres;
using pg::ClusterHostType;

Measure::Measure(pg::ClusterPtr pg)
	: _pg{std::move(pg)}
{}

const pg::Query kSelect{
	"SELECT id, project_id, name, is_deleted FROM project.measure WHERE id = $1",
	pg::Query::Name{"select_measure"},
};

model::Measure Measure::Select(int id) {
	auto res = _pg->Execute(ClusterHostType::kMaster, kSelect, id);
	if (res.IsEmpty())
		throw errors::NotFound404{};

	return res.AsSingleRow<model::Measure>(pg::kRowTag);
}

const pg::Query kInsert{
	"INSERT INTO project.measure (project_id, name, is_deleted) "
	"VALUES ($1, $2, $3) RETURNING id",
	pg::Query::Name{"insert_measure"},
};

int Measure::Insert(
		const boost::uuids::uuid& projectId,
		const std::string& name,
		bool isDeleted)
{
	const auto res =_pg->Execute(ClusterHostType::kMaster, kInsert, projectId, name, isDeleted);
	return res.AsSingleRow<int>();
}

const pg::Query kUpdate {
	"UPDATE project.measure SET project_id = $2, name = $3, is_deleted = $4 "
	"WHERE id = $1",
	pg::Query::Name{"update_measure"},
};

void Measure::Update(const model::Measure& measure) {
	auto res = _pg->Execute(ClusterHostType::kMaster, kUpdate, measure.id, measure.projectId, measure.name, measure.isDeleted);
	if (!res.RowsAffected())
		throw errors::NotFound404();
}

const pg::Query kDelete {
	"DELETE FROM project.measure WHERE id = $1",
	pg::Query::Name{"delete_measure"},
};

void Measure::Delete(int id) {
	auto res = _pg->Execute(ClusterHostType::kMaster, kDelete, id);
	if (!res.RowsAffected())
		throw errors::NotFound404();
}

const pg::Query kSelectSections{
	"SELECT id, project_id, name, is_deleted FROM project.measure "
	"OFFSET $1 LIMIT $2",
	pg::Query::Name{"select_measures"},
};

const pg::Query kCount{
	"SELECT COUNT(*) FROM project.measure",
	pg::Query::Name{"count_measures"},
};

PagingResult<model::Measure> Measure::GetList(int start, int limit) {
	PagingResult<model::Measure> data;

	auto trx = _pg->Begin(pg::Transaction::RO);
	auto res = trx.Execute(kSelectSections, start, limit);
	data.items = res.AsContainer<decltype(data.items)>(pg::kRowTag);
	res = trx.Execute(kCount);
	data.total = res.AsSingleRow<int64_t>();
	trx.Commit();
	return data;
}

} // namespace svetit::project::table