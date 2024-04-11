#include "table_device_plugin_param.hpp"
#include <shared/errors.hpp>
#include <shared/paging.hpp>

#include <userver/components/component_config.hpp>
#include <userver/components/component_context.hpp>
#include <userver/yaml_config/merge_schemas.hpp>
#include <userver/storages/postgres/component.hpp>

namespace svetit::project::table {

namespace pg = storages::postgres;
using pg::ClusterHostType;

DevicePluginParam::DevicePluginParam(pg::ClusterPtr pg)
	: _pg{std::move(pg)}
{}

const pg::Query kSelect{
	"SELECT device_id, param_id FROM project.device_plugin_param WHERE device_id = $1 AND param_id = $2",
	pg::Query::Name{"select_device_plugin_param"},
};

model::DevicePluginParam DevicePluginParam::Select(int deviceId, int paramId) {
	auto res = _pg->Execute(ClusterHostType::kMaster, kSelect, deviceId, paramId);
	if (res.IsEmpty())
		throw errors::NotFound404{};

	return res.AsSingleRow<model::DevicePluginParam>(pg::kRowTag);
}

const pg::Query kInsert{
	"INSERT INTO project.device_plugin_param (device_id, param_id) "
	"VALUES ($1, $2) RETURNING device_id, param_id",
	pg::Query::Name{"insert_device_plugin_param"},
};

void DevicePluginParam::Insert(int deviceId, int paramId) {
	const auto res =_pg->Execute(ClusterHostType::kMaster, kInsert, deviceId, paramId);
	// is needed to return vector or pair with inserted row primary key?
}

const pg::Query kDelete {
	"DELETE FROM project.device_plugin_param WHERE device_id = $1 AND param_id = $2",
	pg::Query::Name{"delete_device_plugin_param"},
};

void DevicePluginParam::Delete(int deviceId, int paramId) {
	auto res = _pg->Execute(ClusterHostType::kMaster, kDelete, deviceId, paramId);
	if (!res.RowsAffected())
		throw errors::NotFound404();
}

const pg::Query kSelectDevicePluginParams{
	"SELECT device_id, param_id FROM project.device_plugin_param "
	"OFFSET $1 LIMIT $2",
	pg::Query::Name{"select_device_plugin_params"},
};

const pg::Query kCount{
	"SELECT COUNT(*) FROM project.device_plugin_param",
	pg::Query::Name{"count_device_plugin_params"},
};

PagingResult<model::DevicePluginParam> DevicePluginParam::GetList(int start, int limit) {
	PagingResult<model::DevicePluginParam> data;

	auto trx = _pg->Begin(pg::Transaction::RO);
	auto res = trx.Execute(kSelectDevicePluginParams, start, limit);
	data.items = res.AsContainer<decltype(data.items)>(pg::kRowTag);
	res = trx.Execute(kCount);
	data.total = res.AsSingleRow<int64_t>();
	trx.Commit();
	return data;
}

} // namespace svetit::project::table