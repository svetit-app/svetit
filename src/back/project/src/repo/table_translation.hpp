#pragma once

#include "../model/translation.hpp"
#include <shared/paging.hpp>

#include <userver/components/loggable_component_base.hpp>
#include <userver/utest/using_namespace_userver.hpp>
#include <userver/storages/postgres/cluster.hpp>

namespace svetit::project::table {

class Translation final {
public:
	explicit Translation(storages::postgres::ClusterPtr pg);
	model::Translation Get(const boost::uuids::uuid& spaceId, int64_t id);
	int64_t Create(const model::Translation& translation);
	void Update(const model::Translation& translation);
	void Delete(const boost::uuids::uuid& spaceId, int64_t id);
	PagingResult<model::Translation> GetList(const boost::uuids::uuid& spaceId, int start, int limit);
private:
	storages::postgres::ClusterPtr _pg;
};

} // namespace svetit::project::table
