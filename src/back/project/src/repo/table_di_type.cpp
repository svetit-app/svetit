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
	"SELECT id, measure_id, save_timer_id, key, name, mode, save_algorithm FROM project.di_type WHERE id = $1",
	pg::Query::Name{"select_di_type"}
};

model::DiType DiType::Get(const boost::uuids::uuid& spaceId, int64_t id) {
	auto res = _pg->Execute(ClusterHostType::kMaster, kGet, id);
	if (res.IsEmpty())
		throw errors::NotFound404{};

	return res.AsSingleRow<model::DiType>(pg::kRowTag);
}

const pg::Query kCreate{
	"INSERT INTO project.di_type (measure_id, save_timer_id, key, name, mode, save_algorithm) "
	"VALUES ($1, $2, $3, $4, $5, $6)"
	"RETURNING id",
	pg::Query::Name{"insert_di_type"},
};

int64_t DiType::Create(const model::DiType& diType)
{
	auto res = _pg->Execute(ClusterHostType::kMaster, kCreate, diType.measureId, diType.saveTimerId, diType.key, diType.name, diType.mode, diType.saveAlgorithm);
	return res.AsSingleRow<int64_t>();
}

const pg::Query kUpdate {
	"UPDATE project.di_type SET measure_id = $2, save_timer_id = $3, key = $4, name = $5, mode = $6, save_algorithm = $7 "
	"WHERE id = $1",
	pg::Query::Name{"update_di_type"},
};

void DiType::Update(const model::DiType& diType) {
	auto res = _pg->Execute(ClusterHostType::kMaster, kUpdate, diType.id, diType.measureId, diType.saveTimerId, diType.key, diType.name, diType.mode, diType.saveAlgorithm);
	if (!res.RowsAffected())
		throw errors::NotFound404();
}

const pg::Query kDelete {
	"DELETE FROM project.di_type WHERE id = $1",
	pg::Query::Name{"delete_di_type"},
};

void DiType::Delete(const boost::uuids::uuid& spaceId, int64_t id) {
	auto res = _pg->Execute(ClusterHostType::kMaster, kDelete, id);
	if (!res.RowsAffected())
		throw errors::NotFound404();
}

const pg::Query kSelectDiTypes{
	"SELECT id, measure_id, save_timer_id, key, name, mode, save_algorithm FROM project.di_type "
	"OFFSET $1 LIMIT $2",
	pg::Query::Name{"select_di_types"},
};

const pg::Query kCount{
	"SELECT COUNT(*) FROM project.di_type",
	pg::Query::Name{"count_di_types"},
};

PagingResult<model::DiType> DiType::GetList(const boost::uuids::uuid& spaceId, int start, int limit) {
	PagingResult<model::DiType> data;

	auto trx = _pg->Begin(pg::Transaction::RO);
	auto res = trx.Execute(kSelectDiTypes, start, limit);
	data.items = res.AsContainer<decltype(data.items)>(pg::kRowTag);
	res = trx.Execute(kCount);
	data.total = res.AsSingleRow<int64_t>();
	trx.Commit();
	return data;
}

} // namespace svetit::project::table
