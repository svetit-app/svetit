#include "table_di_type.hpp"
#include <shared/errors.hpp>
#include <shared/paging.hpp>

#include <userver/components/component_config.hpp>
#include <userver/components/component_context.hpp>
#include <userver/yaml_config/merge_schemas.hpp>
#include <userver/storages/postgres/component.hpp>

namespace svetit::project::table {

namespace pg = storages::postgres;
using pg::ClusterHostType;

DiType::DiType(pg::ClusterPtr pg)
	: _pg{std::move(pg)}
{}

const pg::Query kGet{
	"SELECT id, space_id, project_id, measure_id, save_timer_id, key, name, mode, save_algorithm FROM project.di_type WHERE id = $1 AND space_id = $2",
	pg::Query::Name{"select_di_type"}
};

model::DiType DiType::Get(const boost::uuids::uuid& spaceId, int64_t id) {
	auto res = _pg->Execute(ClusterHostType::kMaster, kGet, id, spaceId);
	if (res.IsEmpty())
		throw errors::NotFound404{};

	return res.AsSingleRow<model::DiType>(pg::kRowTag);
}

const pg::Query kCreate{
	"INSERT INTO project.di_type (space_id, project_id, measure_id, save_timer_id, key, name, mode, save_algorithm) "
	"VALUES ($1, $2, $3, $4, $5, $6, $7, $8) "
	"RETURNING id",
	pg::Query::Name{"insert_di_type"},
};

int64_t DiType::Create(const model::DiType& item)
{
	auto res = _pg->Execute(ClusterHostType::kMaster, kCreate, item.spaceId, item.projectId, item.measureId, item.saveTimerId, item.key, item.name, item.mode, item.saveAlgorithm);
	return res.AsSingleRow<int64_t>();
}

const pg::Query kUpdate {
	"UPDATE project.di_type SET project_id = $3, measure_id = $4, save_timer_id = $5, key = $6, name = $7, mode = $8, save_algorithm = $9 "
	"WHERE id = $1 AND space_id = $2",
	pg::Query::Name{"update_di_type"},
};

void DiType::Update(const model::DiType& item) {
	auto res = _pg->Execute(ClusterHostType::kMaster, kUpdate, item.id, item.spaceId, item.projectId, item.measureId, item.saveTimerId, item.key, item.name, item.mode, item.saveAlgorithm);
	if (!res.RowsAffected())
		throw errors::NotFound404();
}

const pg::Query kDelete {
	"DELETE FROM project.di_type WHERE id = $1 AND space_id = $2",
	pg::Query::Name{"delete_di_type"},
};

void DiType::Delete(const boost::uuids::uuid& spaceId, int64_t id) {
	auto res = _pg->Execute(ClusterHostType::kMaster, kDelete, id, spaceId);
	if (!res.RowsAffected())
		throw errors::NotFound404();
}

const pg::Query kSelectDiTypes{
	"SELECT id, space_id, project_id, measure_id, save_timer_id, key, name, mode, save_algorithm FROM project.di_type "
	"WHERE space_id = $1 AND project_id = $2 "
	"OFFSET $3 LIMIT $4",
	pg::Query::Name{"select_di_types"},
};

const pg::Query kCount{
	"SELECT COUNT(*) FROM project.di_type "
	"WHERE space_id = $1 AND project_id = $2",
	pg::Query::Name{"count_di_types"},
};

PagingResult<model::DiType> DiType::GetList(const boost::uuids::uuid& spaceId, const boost::uuids::uuid& projectId, int start, int limit) {
	PagingResult<model::DiType> data;

	auto trx = _pg->Begin(pg::Transaction::RO);
	auto res = trx.Execute(kSelectDiTypes, spaceId, projectId, start, limit);
	data.items = res.AsContainer<decltype(data.items)>(pg::kRowTag);
	res = trx.Execute(kCount, spaceId, projectId);
	data.total = res.AsSingleRow<int64_t>();
	trx.Commit();
	return data;
}

} // namespace svetit::project::table
