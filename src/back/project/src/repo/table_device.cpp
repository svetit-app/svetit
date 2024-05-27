#include "table_device.hpp"
#include <shared/errors.hpp>
#include <shared/paging.hpp>

#include <userver/components/component_config.hpp>
#include <userver/components/component_context.hpp>
#include <userver/yaml_config/merge_schemas.hpp>
#include <userver/storages/postgres/component.hpp>

namespace svetit::project::table {

namespace pg = storages::postgres;
using pg::ClusterHostType;

Device::Device(pg::ClusterPtr pg)
	: _pg{std::move(pg)}
{}

const pg::Query kGet{
	"SELECT id, space_id, project_id, plugin_id, name, check_interval_msec FROM project.device WHERE id = $1 AND space_id = $2",
	pg::Query::Name{"select_device"},
};

model::Device Device::Get(const boost::uuids::uuid& spaceId, int64_t id) {
	auto res = _pg->Execute(ClusterHostType::kMaster, kGet, id, spaceId);
	if (res.IsEmpty())
		throw errors::NotFound404{};

	return res.AsSingleRow<model::Device>(pg::kRowTag);
}

const pg::Query kCreate{
	"INSERT INTO project.device (space_id, project_id, plugin_id, name, check_interval_msec) "
	"VALUES ($1, $2, $3, $4, $5)"
	"RETURNING id",
	pg::Query::Name{"insert_device"},
};

int64_t Device::Create(const model::Device& item)
{
	auto res = _pg->Execute(ClusterHostType::kMaster, kCreate, item.spaceId, item.projectId, item.pluginId, item.name, item.checkIntervalMsec);
	return res.AsSingleRow<int64_t>();
}

const pg::Query kUpdate {
	"UPDATE project.device SET project_id = $3, plugin_id = $4, name = $5, check_interval_msec = $6 "
	"WHERE id = $1 AND space_id = $2",
	pg::Query::Name{"update_device"},
};

void Device::Update(const model::Device& item) {
	auto res = _pg->Execute(ClusterHostType::kMaster, kUpdate, item.id, item.spaceId, item.projectId, item.pluginId, item.name, item.checkIntervalMsec);
	if (!res.RowsAffected())
		throw errors::NotFound404();
}

const pg::Query kDelete {
	"DELETE FROM project.device WHERE id = $1 AND space_id = $2",
	pg::Query::Name{"delete_device"},
};

void Device::Delete(const boost::uuids::uuid& spaceId, int64_t id) {
	auto res = _pg->Execute(ClusterHostType::kMaster, kDelete, id, spaceId);
	if (!res.RowsAffected())
		throw errors::NotFound404();
}

const pg::Query kSelectDevices{
	"SELECT id, space_id, project_id, plugin_id, name, check_interval_msec FROM project.device "
	"WHERE space_id = $1 AND project_id = $2"
	"OFFSET $3 LIMIT $4",
	pg::Query::Name{"select_devices"},
};

const pg::Query kCount{
	"SELECT COUNT(*) FROM project.device",
	pg::Query::Name{"count_devices"},
};

PagingResult<model::Device> Device::GetList(const boost::uuids::uuid& spaceId, const boost::uuids::uuid& projectId, int start, int limit) {
	PagingResult<model::Device> data;

	auto trx = _pg->Begin(pg::Transaction::RO);
	auto res = trx.Execute(kSelectDevices, spaceId, projectId, start, limit);
	data.items = res.AsContainer<decltype(data.items)>(pg::kRowTag);
	res = trx.Execute(kCount);
	data.total = res.AsSingleRow<int64_t>();
	trx.Commit();
	return data;
}

} // namespace svetit::project::table
