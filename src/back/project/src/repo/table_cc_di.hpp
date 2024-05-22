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
	model::CcDi Get(const boost::uuids::uuid& spaceId, int64_t ccId, int64_t diId);
	void Insert(int64_t ccId, int64_t diId);
	void Delete(const boost::uuids::uuid& spaceId, int64_t ccId, int64_t diId);
	PagingResult<model::CcDi> GetList(const boost::uuids::uuid& spaceId, int start, int limit);
private:
	storages::postgres::ClusterPtr _pg;
};

} // namespace svetit::project::table
