#pragma once

#include "../model/cc_type_di_type.hpp"
#include <shared/paging.hpp>

#include <userver/components/loggable_component_base.hpp>
#include <userver/utest/using_namespace_userver.hpp>
#include <userver/storages/postgres/cluster.hpp>

namespace svetit::project::table {

class CcTypeDiType final {
public:
	explicit CcTypeDiType(storages::postgres::ClusterPtr pg);
	model::CcTypeDiType Get(int64_t ccTypeId, int64_t diTypeId);
	void Insert(int64_t ccTypeId, int64_t diTypeId);
	void Delete(int64_t ccTypeId, int64_t diTypeId);
	PagingResult<model::CcTypeDiType> GetList(int start, int limit);
private:
	storages::postgres::ClusterPtr _pg;
};

} // namespace svetit::project::table
