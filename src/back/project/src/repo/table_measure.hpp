#pragma once

#include "../model/measure.hpp"
#include <shared/paging.hpp>

#include <userver/components/loggable_component_base.hpp>
#include <userver/utest/using_namespace_userver.hpp>
#include <userver/storages/postgres/cluster.hpp>

namespace svetit::project::table {

class Measure final {
public:
	explicit Measure(storages::postgres::ClusterPtr pg);
	model::Measure Get(const boost::uuids::uuid& spaceId, int64_t id);
	int64_t Create(const model::Measure& item);
	void Update(const model::Measure& item);
	void Delete(const boost::uuids::uuid& spaceId, int64_t id);
	PagingResult<model::Measure> GetList(const boost::uuids::uuid& spaceId, const boost::uuids::uuid& projectId, int start, int limit);
private:
	storages::postgres::ClusterPtr _pg;
};

} // namespace svetit::project::table
