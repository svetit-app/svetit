#pragma once

#include "../model/cc_param.hpp"
#include <shared/paging.hpp>

#include <userver/components/loggable_component_base.hpp>
#include <userver/utest/using_namespace_userver.hpp>
#include <userver/storages/postgres/cluster.hpp>

namespace svetit::project::table {

class CcParam final {
public:
	explicit CcParam(storages::postgres::ClusterPtr pg);
	model::CcParam Get(int ccId, int paramId);
	void Insert(int ccId, int paramId);
	void Delete(int ccId, int paramId);
	PagingResult<model::CcParam> GetList(int start, int limit);
private:
	storages::postgres::ClusterPtr _pg;
};

} // namespace svetit::project::table
