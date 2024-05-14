#pragma once

#include "../model/cc_di.hpp"
#include <shared/paging.hpp>

#include <userver/components/loggable_component_base.hpp>
#include <userver/utest/using_namespace_userver.hpp>
#include <userver/storages/postgres/cluster.hpp>

namespace svetit::project::table {

class CcDi final {
public:
	explicit CcDi(storages::postgres::ClusterPtr pg);
	model::CcDi Get(int ccId, int diId);
	void Insert(int ccId, int diId);
	void Delete(int ccId, int diId);
	PagingResult<model::CcDi> GetList(int start, int limit);
private:
	storages::postgres::ClusterPtr _pg;
};

} // namespace svetit::project::table
