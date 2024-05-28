#pragma once

#include "../model/device_plugin_param.hpp"
#include <shared/paging.hpp>

#include <userver/components/loggable_component_base.hpp>
#include <userver/utest/using_namespace_userver.hpp>
#include <userver/storages/postgres/cluster.hpp>

namespace svetit::project::table {

class DevicePluginParam final {
public:
	explicit DevicePluginParam(storages::postgres::ClusterPtr pg);
	model::DevicePluginParam Get(const boost::uuids::uuid& spaceId, int64_t deviceId, int64_t paramId);
	void Create(const model::DevicePluginParam& item);
	void Update(const model::DevicePluginParam&);
	void Delete(const boost::uuids::uuid& spaceId, int64_t deviceId, int64_t paramId);
	PagingResult<model::DevicePluginParam> GetList(const boost::uuids::uuid& spaceId, int64_t deviceId, int start, int limit);
private:
	storages::postgres::ClusterPtr _pg;
};

} // namespace svetit::project::table
