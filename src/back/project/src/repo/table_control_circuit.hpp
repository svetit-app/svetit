#pragma once

#include "../model/control_circuit.hpp"
#include <shared/paging.hpp>

#include <userver/components/loggable_component_base.hpp>
#include <userver/utest/using_namespace_userver.hpp>
#include <userver/storages/postgres/cluster.hpp>

namespace svetit::project::table {

class ControlCircuit final {
public:
	explicit ControlCircuit(storages::postgres::ClusterPtr pg);
	model::ControlCircuit Get(const boost::uuids::uuid& spaceId, int64_t id);
	int64_t Create(const model::ControlCircuit& controlCircuit);
	void Update(const model::ControlCircuit& controlCircuit);
	void Delete(const boost::uuids::uuid& spaceId, int64_t id);
	PagingResult<model::ControlCircuit> GetList(const boost::uuids::uuid& spaceId, int start, int limit);
private:
	storages::postgres::ClusterPtr _pg;
};

} // namespace svetit::project::table
