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

const pg::Query kSelect{
	"SELECT id, project_id, plugin_id, name, check_interval_msec, is_deleted FROM project.device WHERE id = $1",
	pg::Query::Name{"select_device"},
};

model::Device Device::Select(int id) {
	auto res = _pg->Execute(ClusterHostType::kMaster, kSelect, id);
	if (res.IsEmpty())
		throw errors::NotFound404{};

	return res.AsSingleRow<model::Device>(pg::kRowTag);
}

const pg::Query kInsert{
	"INSERT INTO project.device (project_id, plugin_id, name, check_interval_msec, is_deleted) "
	"VALUES ($1, $2, $3, $4, $5) RETURNING id",
	pg::Query::Name{"insert_device"},
};

int Device::Insert(
		const boost::uuids::uuid& projectId,
		int pluginId,
		const std::string& name,
		int checkIntervalMsec,
		bool isDeleted)
{
	const auto res =_pg->Execute(ClusterHostType::kMaster, kInsert, projectId, pluginId, name, checkIntervalMsec, isDeleted);
	return res.AsSingleRow<int>();
}

const pg::Query kUpdate {
	"UPDATE project.device SET project_id = $2, plugin_id = $3, name = $4, check_interval_msec = $5, is_deleted = $6 "
	"WHERE id = $1",
	pg::Query::Name{"update_device"},
};

void Device::Update(const model::Device& device) {
	auto res = _pg->Execute(ClusterHostType::kMaster, kUpdate, device.id, device.projectId, device.pluginId, device.name, device.checkIntervalMsec, device.isDeleted);
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
	"SELECT id, project_id, plugin_id, name, check_interval_msec, is_deleted FROM project.device "
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