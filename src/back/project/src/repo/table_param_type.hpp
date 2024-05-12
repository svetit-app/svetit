#pragma once

#include "../model/param_type.hpp"
#include <shared/paging.hpp>

#include <userver/components/loggable_component_base.hpp>
#include <userver/utest/using_namespace_userver.hpp>
#include <userver/storages/postgres/cluster.hpp>

namespace svetit::project::table {

class ParamType final {
public:
	explicit ParamType(storages::postgres::ClusterPtr pg);
	model::ParamType Get(int64_t id);
	int64_t Create(const model::ParamType& item);
	void Update(const model::ParamType& paramType);
	void Delete(int64_t id);
	PagingResult<model::ParamType> GetList(int start, int limit);
private:
	storages::postgres::ClusterPtr _pg;
};

} // namespace svetit::project::table
