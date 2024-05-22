#pragma once

#include "../model/cc_type_param.hpp"
#include <shared/paging.hpp>

#include <userver/components/loggable_component_base.hpp>
#include <userver/utest/using_namespace_userver.hpp>
#include <userver/storages/postgres/cluster.hpp>

namespace svetit::project::table {

class CcTypeParam final {
public:
	explicit CcTypeParam(storages::postgres::ClusterPtr pg);
	model::CcTypeParam Get(const boost::uuids::uuid& spaceId, int64_t ccTypeId, int64_t paramId);
	void Insert(int64_t ccTypeId, int64_t paramId);
	void Delete(const boost::uuids::uuid& spaceId, int64_t ccTypeId, int64_t paramId);
	PagingResult<model::CcTypeParam> GetList(const boost::uuids::uuid& spaceId, int start, int limit);
private:
	storages::postgres::ClusterPtr _pg;
};

} // namespace svetit::project::table
