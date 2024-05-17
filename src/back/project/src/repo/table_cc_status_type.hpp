#pragma once

#include "../model/cc_status_type.hpp"
#include <shared/paging.hpp>

#include <userver/components/loggable_component_base.hpp>
#include <userver/utest/using_namespace_userver.hpp>
#include <userver/storages/postgres/cluster.hpp>

namespace svetit::project::table {

class CcStatusType final {
public:
	explicit CcStatusType(storages::postgres::ClusterPtr pg);
	model::CcStatusType Get(int64_t id);
	int64_t Create(const model::CcStatusType& ccStatusType);
	void Update(const model::CcStatusType& ccStatusType);
	void Delete(int64_t id);
	PagingResult<model::CcStatusType> GetList(int start, int limit);
private:
	storages::postgres::ClusterPtr _pg;
};

} // namespace svetit::project::table
