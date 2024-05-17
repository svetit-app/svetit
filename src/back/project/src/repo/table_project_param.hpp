#pragma once

#include "../model/project_param.hpp"
#include <shared/paging.hpp>

#include <userver/components/loggable_component_base.hpp>
#include <userver/utest/using_namespace_userver.hpp>
#include <userver/storages/postgres/cluster.hpp>

namespace svetit::project::table {

class ProjectParam final {
public:
	explicit ProjectParam(storages::postgres::ClusterPtr pg);
	model::ProjectParam Get(const boost::uuids::uuid& projectId, int64_t paramId);
	void Insert(const boost::uuids::uuid& projectId, int64_t paramId);
	void Delete(const boost::uuids::uuid& projectId, int64_t paramId);
	PagingResult<model::ProjectParam> GetList(int start, int limit);
private:
	storages::postgres::ClusterPtr _pg;
};

} // namespace svetit::project::table
