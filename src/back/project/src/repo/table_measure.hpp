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
	model::Measure Select(int id);
	void Insert(
		const boost::uuids::uuid& projectId,
		const std::string& name);
	void Update(const model::Measure& measure);
	void Delete(int id);
	PagingResult<model::Measure> GetList(int start, int limit);
private:
	storages::postgres::ClusterPtr _pg;
};

} // namespace svetit::project::table
