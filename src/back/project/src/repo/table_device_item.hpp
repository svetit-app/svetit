#pragma once

#include "../model/device_item.hpp"
#include <boost/uuid/uuid.hpp>
#include <shared/paging.hpp>

#include <userver/components/loggable_component_base.hpp>
#include <userver/utest/using_namespace_userver.hpp>
#include <userver/storages/postgres/cluster.hpp>

namespace svetit::project::table {

class DeviceItem final {
public:
	explicit DeviceItem(storages::postgres::ClusterPtr pg);
	model::DeviceItem Get(const boost::uuids::uuid& spaceId, int64_t id);
	int64_t Create(const model::DeviceItem& item);
	void Update(const model::DeviceItem& item);
	void Delete(const boost::uuids::uuid& spaceId, int64_t id);
	PagingResult<model::DeviceItem> GetList(const boost::uuids::uuid& spaceId, int64_t deviceId, int start, int limit);
private:
	storages::postgres::ClusterPtr _pg;
};

} // namespace svetit::project::table
