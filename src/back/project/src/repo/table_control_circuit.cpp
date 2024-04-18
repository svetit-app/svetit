#include "table_control_circuit.hpp"
#include <shared/errors.hpp>
#include <shared/paging.hpp>

#include <userver/components/component_config.hpp>
#include <userver/components/component_context.hpp>
#include <userver/yaml_config/merge_schemas.hpp>
#include <userver/storages/postgres/component.hpp>

namespace svetit::project::table {

namespace pg = storages::postgres;
using pg::ClusterHostType;

ControlCircuit::ControlCircuit(pg::ClusterPtr pg)
	: _pg{std::move(pg)}
{}

const pg::Query kSelect{
	"SELECT id, type_id, section_id, name FROM project.control_circuit WHERE id = $1",
	pg::Query::Name{"select_control_circuit"}
};

model::ControlCircuit ControlCircuit::Select(int id) {
	auto res = _pg->Execute(ClusterHostType::kMaster, kSelect, id);
	if (res.IsEmpty())
		throw errors::NotFound404{};

	return res.AsSingleRow<model::ControlCircuit>(pg::kRowTag);
}

const pg::Query kInsert{
	"INSERT INTO project.control_circuit (type_id, section_id, name) "
	"VALUES ($1, $2, $3)",
	pg::Query::Name{"insert_control_circuit"},
};

void ControlCircuit::Insert(
	int typeId,
	int sectionId,
	const std::string& name)
{
	_pg->Execute(ClusterHostType::kMaster, kInsert, typeId, sectionId, name);
}

const pg::Query kUpdate {
	"UPDATE project.control_circuit SET type_id = $2, section_id = $3, name = $4 "
	"WHERE id = $1",
	pg::Query::Name{"update_control_circuit"},
};

void ControlCircuit::Update(const model::ControlCircuit& controlCircuit) {
	auto res = _pg->Execute(ClusterHostType::kMaster, kUpdate, controlCircuit.id, controlCircuit.typeId, controlCircuit.sectionId, controlCircuit.name);
	if (!res.RowsAffected())
		throw errors::NotFound404();
}

const pg::Query kDelete {
	"DELETE FROM project.control_circuit WHERE id = $1",
	pg::Query::Name{"delete_control_circuit"},
};

void ControlCircuit::Delete(int id) {
	auto res = _pg->Execute(ClusterHostType::kMaster, kDelete, id);
	if (!res.RowsAffected())
		throw errors::NotFound404();
}

const pg::Query kSelectControlCircuits{
	"SELECT id, type_id, section_id, name FROM project.control_circuit "
	"OFFSET $1 LIMIT $2",
	pg::Query::Name{"select_control_circuits"},
};

const pg::Query kCount{
	"SELECT COUNT(*) FROM project.control_circuit",
	pg::Query::Name{"count_control_circuits"},
};

PagingResult<model::ControlCircuit> ControlCircuit::GetList(int start, int limit) {
	PagingResult<model::ControlCircuit> data;

	auto trx = _pg->Begin(pg::Transaction::RO);
	auto res = trx.Execute(kSelectControlCircuits, start, limit);
	data.items = res.AsContainer<decltype(data.items)>(pg::kRowTag);
	res = trx.Execute(kCount);
	data.total = res.AsSingleRow<int64_t>();
	trx.Commit();
	return data;
}

} // namespace svetit::project::table