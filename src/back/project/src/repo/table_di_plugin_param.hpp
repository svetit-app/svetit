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
	model::DiPluginParam Select(int diTypeId, int paramId);
	void Insert(
		int diTypeId,
		int paramId,
		bool isDeleted);
	void Update(const model::DiPluginParam& diPluginParam);
	void Delete(int diTypeId, int paramId);
	PagingResult<model::DiPluginParam> GetList(int start, int limit);
private:
	storages::postgres::ClusterPtr _pg;
};

} // namespace svetit::project::table
