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
	model::CcParam Get(const boost::uuids::uuid& spaceId, int64_t ccId, int64_t paramId);
	void Create(const model::CcParam& item);
	void Update(const model::CcParam& item);
	void Delete(const boost::uuids::uuid& spaceId, int64_t ccId, int64_t paramId);
	PagingResult<model::CcParam> GetList(const boost::uuids::uuid& spaceId, int64_t ccId, int start, int limit);
private:
	storages::postgres::ClusterPtr _pg;
};

} // namespace svetit::project::table
