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
	"SELECT id, project_id, plugin_id, name, check_interval_msec FROM project.device WHERE id = $1",
	pg::Query::Name{"select_device"},
};

model::Device Device::Get(int id) {
	auto res = _pg->Execute(ClusterHostType::kMaster, kGet, id);
	if (res.IsEmpty())
		throw errors::NotFound404{};

	return res.AsSingleRow<model::Device>(pg::kRowTag);
}

const pg::Query kInsert{
	"INSERT INTO project.device (project_id, plugin_id, name, check_interval_msec) "
	"VALUES ($1, $2, $3, $4)",
	pg::Query::Name{"insert_device"},
};

void Device::Insert(
		const boost::uuids::uuid& projectId,
		int pluginId,
		const std::string& name,
		int checkIntervalMsec)
{
	_pg->Execute(ClusterHostType::kMaster, kInsert, projectId, pluginId, name, checkIntervalMsec);
}

const pg::Query kUpdate {
	"UPDATE project.device SET project_id = $2, plugin_id = $3, name = $4, check_interval_msec = $5 "
	"WHERE id = $1",
	pg::Query::Name{"update_device"},
};

void Device::Update(const model::Device& device) {
	auto res = _pg->Execute(ClusterHostType::kMaster, kUpdate, device.id, device.projectId, device.pluginId, device.name, device.checkIntervalMsec);
	if (!res.RowsAffected())
		throw errors::NotFound404();
}

const pg::Query kDelete {
	"DELETE FROM project.device WHERE id = $1",
	pg::Query::Name{"delete_device"},
};

void Device::Delete(int id) {
	auto res = _pg->Execute(ClusterHostType::kMaster, kDelete, id);
	if (!res.RowsAffected())
		throw errors::NotFound404();
}

const pg::Query kSelectDevices{
	"SELECT id, project_id, plugin_id, name, check_interval_msec FROM project.device "
	"OFFSET $1 LIMIT $2",
	pg::Query::Name{"select_devices"},
};

const pg::Query kCount{
	"SELECT COUNT(*) FROM project.device",
	pg::Query::Name{"count_devices"},
};

PagingResult<model::Device> Device::GetList(int start, int limit) {
	PagingResult<model::Device> data;

	auto trx = _pg->Begin(pg::Transaction::RO);
	auto res = trx.Execute(kSelectDevices, start, limit);
	data.items = res.AsContainer<decltype(data.items)>(pg::kRowTag);
	res = trx.Execute(kCount);
	data.total = res.AsSingleRow<int64_t>();
	trx.Commit();
	return data;
}

} // namespace svetit::project::table
