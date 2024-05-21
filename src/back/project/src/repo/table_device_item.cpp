#include "table_device_item.hpp"
#include <shared/errors.hpp>
#include <shared/paging.hpp>

#include <userver/components/component_config.hpp>
#include <userver/components/component_context.hpp>
#include <userver/yaml_config/merge_schemas.hpp>
#include <userver/storages/postgres/component.hpp>

namespace svetit::project::table {

namespace pg = storages::postgres;
using pg::ClusterHostType;

DeviceItem::DeviceItem(pg::ClusterPtr pg)
	: _pg{std::move(pg)}
{}

const pg::Query kGet{
	"SELECT id, device_id, type_id, name FROM project.device_item WHERE id = $1",
	pg::Query::Name{"select_device_item"}
};

model::DeviceItem DeviceItem::Get(int64_t id) {
	auto res = _pg->Execute(ClusterHostType::kMaster, kGet, id);
	if (res.IsEmpty())
		throw errors::NotFound404{};

	return res.AsSingleRow<model::DeviceItem>(pg::kRowTag);
}

const pg::Query kCreate{
	"INSERT INTO project.device_item (device_id, type_id, name) "
	"VALUES ($1, $2, $3)"
	"RETURNING id",
	pg::Query::Name{"insert_device_item"},
};

int64_t DeviceItem::Create(const model::DeviceItem& deviceItem)
{
	auto res = _pg->Execute(ClusterHostType::kMaster, kCreate, deviceItem.deviceId, deviceItem.typeId, deviceItem.name);
	return res.AsSingleRow<int64_t>();
}

const pg::Query kUpdate {
	"UPDATE project.device_item SET device_id = $2, type_id = $3, name = $4 "
	"WHERE id = $1",
	pg::Query::Name{"update_device_item"},
};

void DeviceItem::Update(const model::DeviceItem& deviceItem) {
	auto res = _pg->Execute(ClusterHostType::kMaster, kUpdate, deviceItem.id, deviceItem.deviceId, deviceItem.typeId, deviceItem.name);
	if (!res.RowsAffected())
		throw errors::NotFound404();
}

const pg::Query kDelete {
	"DELETE FROM project.device_item WHERE id = $1",
	pg::Query::Name{"delete_device_item"},
};

void DeviceItem::Delete(int64_t id) {
	auto res = _pg->Execute(ClusterHostType::kMaster, kDelete, id);
	if (!res.RowsAffected())
		throw errors::NotFound404();
}

const pg::Query kSelectDeviceItems{
	"SELECT id, device_id, type_id, name FROM project.device_item "
	"WHERE device_id=$1 "
	"OFFSET $2 LIMIT $3",
	pg::Query::Name{"select_device_items"},
};

const pg::Query kCount{
	"SELECT COUNT(*) FROM project.device_item",
	pg::Query::Name{"count_device_items"},
};

PagingResult<model::DeviceItem> DeviceItem::GetList(const boost::uuids::uuid& spaceId, int64_t deviceId, int start, int limit) {
	PagingResult<model::DeviceItem> data;

	// TODO: use spaceId in query
	auto trx = _pg->Begin(pg::Transaction::RO);
	auto res = trx.Execute(kSelectDeviceItems, deviceId, start, limit);
	data.items = res.AsContainer<decltype(data.items)>(pg::kRowTag);
	res = trx.Execute(kCount);
	data.total = res.AsSingleRow<int64_t>();
	trx.Commit();
	return data;
}

} // namespace svetit::project::table
