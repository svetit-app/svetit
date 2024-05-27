#include "table_save_timer.hpp"
#include <shared/errors.hpp>
#include <shared/paging.hpp>

#include <userver/components/component_config.hpp>
#include <userver/components/component_context.hpp>
#include <userver/yaml_config/merge_schemas.hpp>
#include <userver/storages/postgres/component.hpp>

namespace svetit::project::table {

namespace pg = storages::postgres;
using pg::ClusterHostType;

SaveTimer::SaveTimer(pg::ClusterPtr pg)
	: _pg{std::move(pg)}
{}

const pg::Query kGet{
	"SELECT id, space_id, project_id, interval_msec FROM project.save_timer WHERE id = $1 AND space_id = $2",
	pg::Query::Name{"select_save_timer"},
};

model::SaveTimer SaveTimer::Get(const boost::uuids::uuid& spaceId, int64_t id) {
	auto res = _pg->Execute(ClusterHostType::kMaster, kGet, id, spaceId);
	if (res.IsEmpty())
		throw errors::NotFound404{};

	return res.AsSingleRow<model::SaveTimer>(pg::kRowTag);
}

const pg::Query kCreate{
	"INSERT INTO project.save_timer (space_id, project_id, interval_msec) "
	"VALUES ($1, $2, $3)"
	"RETURNING id",
	pg::Query::Name{"insert_save_timer"},
};

int64_t SaveTimer::Create(const model::SaveTimer& item)
{
	auto res = _pg->Execute(ClusterHostType::kMaster, kCreate, item.spaceId, item.projectId, item.intervalMsec);
	return res.AsSingleRow<int64_t>();
}

const pg::Query kUpdate {
	"UPDATE project.save_timer SET project_id = $3, interval_msec = $4 "
	"WHERE id = $1 AND space_id = $2",
	pg::Query::Name{"update_save_timer"},
};

void SaveTimer::Update(const model::SaveTimer& item) {
	auto res = _pg->Execute(ClusterHostType::kMaster, kUpdate, item.id, item.spaceId, item.projectId, item.intervalMsec);
	if (!res.RowsAffected())
		throw errors::NotFound404();
}

const pg::Query kDelete {
	"DELETE FROM project.save_timer WHERE id = $1 AND space_id = $2",
	pg::Query::Name{"delete_save_timer"},
};

void SaveTimer::Delete(const boost::uuids::uuid& spaceId, int64_t id) {
	auto res = _pg->Execute(ClusterHostType::kMaster, kDelete, id, spaceId);
	if (!res.RowsAffected())
		throw errors::NotFound404();
}

const pg::Query kSelectSaveTimers{
	"SELECT id, space_id, project_id, interval_msec FROM project.save_timer "
	"WHERE space_id = $1 AND project_id = $2"
	"OFFSET $3 LIMIT $4",
	pg::Query::Name{"select_save_timers"},
};

const pg::Query kCount{
	"SELECT COUNT(*) FROM project.save_timer",
	pg::Query::Name{"count_save_timers"},
};

PagingResult<model::SaveTimer> SaveTimer::GetList(const boost::uuids::uuid& spaceId,  const boost::uuids::uuid& projectId, int start, int limit) {
	PagingResult<model::SaveTimer> data;

	auto trx = _pg->Begin(pg::Transaction::RO);
	auto res = trx.Execute(kSelectSaveTimers, spaceId, projectId, start, limit);
	data.items = res.AsContainer<decltype(data.items)>(pg::kRowTag);
	res = trx.Execute(kCount);
	data.total = res.AsSingleRow<int64_t>();
	trx.Commit();
	return data;
}

} // namespace svetit::project::table
