#pragma once

#include "../model/di_plugin_param.hpp"
#include <shared/paging.hpp>

#include <userver/components/loggable_component_base.hpp>
#include <userver/utest/using_namespace_userver.hpp>
#include <userver/storages/postgres/cluster.hpp>

namespace svetit::project::table {

class DiPluginParam final {
public:
	explicit DiPluginParam(storages::postgres::ClusterPtr pg);
	model::DiPluginParam Get(const boost::uuids::uuid& spaceId, int64_t diTypeId, int64_t paramId);
	void Create(const model::DiPluginParam& item);
	void Update(const model::DiPluginParam& item);
	void Delete(const boost::uuids::uuid& spaceId, int64_t diTypeId, int64_t paramId);
	PagingResult<model::DiPluginParam> GetList(const boost::uuids::uuid& spaceId, int64_t diTypeId, int start, int limit);
private:
	storages::postgres::ClusterPtr _pg;
};

} // namespace svetit::project::table
