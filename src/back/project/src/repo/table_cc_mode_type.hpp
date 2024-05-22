#pragma once

#include "../model/cc_mode_type.hpp"
#include <shared/paging.hpp>

#include <userver/components/loggable_component_base.hpp>
#include <userver/utest/using_namespace_userver.hpp>
#include <userver/storages/postgres/cluster.hpp>

namespace svetit::project::table {

class CcModeType final {
public:
	explicit CcModeType(storages::postgres::ClusterPtr pg);
	model::CcModeType Get(int64_t id);
	int64_t Create(const model::CcModeType& ccModeType);
	void Update(const model::CcModeType& ccModeType);
	void Delete(int64_t id);
	PagingResult<model::CcModeType> GetList(const boost::uuids::uuid& spaceId, int64_t ccTypeId, int start, int limit);
private:
	storages::postgres::ClusterPtr _pg;
};

} // namespace svetit::project::table
