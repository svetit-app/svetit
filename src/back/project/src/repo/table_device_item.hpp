#pragma once

#include "../model/device_item.hpp"
#include <shared/paging.hpp>

#include <userver/components/loggable_component_base.hpp>
#include <userver/utest/using_namespace_userver.hpp>
#include <userver/storages/postgres/cluster.hpp>

namespace svetit::project::table {

class DeviceItem final {
public:
	explicit DeviceItem(storages::postgres::ClusterPtr pg);
	model::DeviceItem Select(int id);
	int Insert(
		int deviceId,
		int typeId,
		const std::string& name);
	void Update(const model::DeviceItem& deviceItem);
	void Delete(int id);
	PagingResult<model::DeviceItem> GetList(int start, int limit);
private:
	storages::postgres::ClusterPtr _pg;
};

} // namespace svetit::project::table
