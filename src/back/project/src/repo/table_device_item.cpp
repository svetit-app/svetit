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
	"SELECT id, space_id, device_id, type_id, name FROM project.device_item WHERE space_id = $1 AND id = $2",
	pg::Query::Name{"select_device_item"}
};

model::DeviceItem DeviceItem::Get(const boost::uuids::uuid& spaceId, int64_t id) {
	auto res = _pg->Execute(ClusterHostType::kMaster, kGet, spaceId, id);
	if (res.IsEmpty())
		throw errors::NotFound404{};

	return res.AsSingleRow<model::DeviceItem>(pg::kRowTag);
}

const pg::Query kCreate{
	"INSERT INTO project.device_item (space_id, device_id, type_id, name) "
	"VALUES ($1, $2, $3, $4)"
	"RETURNING id",
	pg::Query::Name{"insert_device_item"},
};

int64_t DeviceItem::Create(const model::DeviceItem& item)
{
	auto res = _pg->Execute(ClusterHostType::kMaster, kCreate, item.spaceId, item.deviceId, item.typeId, item.name);
	return res.AsSingleRow<int64_t>();
}

const pg::Query kUpdate {
	"UPDATE project.device_item SET device_id = $3, type_id = $4, name = $5 "
	"WHERE space_id = $1 AND id = $2",
	pg::Query::Name{"update_device_item"},
};

void DeviceItem::Update(const model::DeviceItem& item) {
	auto res = _pg->Execute(ClusterHostType::kMaster, kUpdate, item.spaceId, item.id, item.deviceId, item.typeId, item.name);
	if (!res.RowsAffected())
		throw errors::NotFound404();
}

const pg::Query kDelete {
	"DELETE FROM project.device_item WHERE space_id = $1 AND id = $2",
	pg::Query::Name{"delete_device_item"},
};

void DeviceItem::Delete(const boost::uuids::uuid& spaceId, int64_t id) {
	auto res = _pg->Execute(ClusterHostType::kMaster, kDelete, spaceId, id);
	if (!res.RowsAffected())
		throw errors::NotFound404();
}

const pg::Query kSelectDeviceItems{
	"SELECT id, space_id, device_id, type_id, name FROM project.device_item "
	"WHERE space_id = $1 AND device_id = $2 "
	"OFFSET $3 LIMIT $4",
	pg::Query::Name{"select_device_items"},
};

const pg::Query kCount{
	"SELECT COUNT(*) FROM project.device_item "
	"WHERE space_id = $1 AND device_id = $2",
	pg::Query::Name{"count_device_items"},
};

PagingResult<model::DeviceItem> DeviceItem::GetList(const boost::uuids::uuid& spaceId, int64_t deviceId, int start, int limit) {
	PagingResult<model::DeviceItem> data;

	auto trx = _pg->Begin(pg::Transaction::RO);
	auto res = trx.Execute(kSelectDeviceItems, spaceId, deviceId, start, limit);
	data.items = res.AsContainer<decltype(data.items)>(pg::kRowTag);
	res = trx.Execute(kCount, spaceId, deviceId);
	data.total = res.AsSingleRow<int64_t>();
	trx.Commit();
	return data;
}

} // namespace svetit::project::table
