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
	model::ControlCircuit Select(int id);
	void Insert(
		int typeId,
		int sectionId,
		const std::string& name);
	void Update(const model::ControlCircuit& controlCircuit);
	void Delete(int id);
	PagingResult<model::ControlCircuit> GetList(int start, int limit);
private:
	storages::postgres::ClusterPtr _pg;
};

} // namespace svetit::project::table
