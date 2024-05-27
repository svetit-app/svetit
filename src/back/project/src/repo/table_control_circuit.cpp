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

const pg::Query kGet{
	"SELECT id, space_id, type_id, section_id, name FROM project.control_circuit WHERE id = $1 AND space_id = $2",
	pg::Query::Name{"select_control_circuit"}
};

model::ControlCircuit ControlCircuit::Get(const boost::uuids::uuid& spaceId, int64_t id) {
	auto res = _pg->Execute(ClusterHostType::kMaster, kGet, id, spaceId);
	if (res.IsEmpty())
		throw errors::NotFound404{};

	return res.AsSingleRow<model::ControlCircuit>(pg::kRowTag);
}

const pg::Query kCreate{
	"INSERT INTO project.control_circuit (space_id, type_id, section_id, name) "
	"VALUES ($1, $2, $3, $4)"
	"RETURNING id",
	pg::Query::Name{"insert_control_circuit"},
};

int64_t ControlCircuit::Create(const model::ControlCircuit& item)
{
	auto res = _pg->Execute(ClusterHostType::kMaster, kCreate, item.spaceId, item.typeId, item.sectionId, item.name);
	return res.AsSingleRow<int64_t>();
}

const pg::Query kUpdate {
	"UPDATE project.control_circuit SET type_id = $3, section_id = $4, name = $5 "
	"WHERE id = $1 AND space_id = $2",
	pg::Query::Name{"update_control_circuit"},
};

void ControlCircuit::Update(const model::ControlCircuit& item) {
	auto res = _pg->Execute(ClusterHostType::kMaster, kUpdate, item.id, item.spaceId, item.typeId, item.sectionId, item.name);
	if (!res.RowsAffected())
		throw errors::NotFound404();
}

const pg::Query kDelete {
	"DELETE FROM project.control_circuit WHERE id = $1 AND space_id = $2",
	pg::Query::Name{"delete_control_circuit"},
};

void ControlCircuit::Delete(const boost::uuids::uuid& spaceId, int64_t id) {
	auto res = _pg->Execute(ClusterHostType::kMaster, kDelete, id, spaceId);
	if (!res.RowsAffected())
		throw errors::NotFound404();
}

const pg::Query kSelectControlCircuits{
	"SELECT id, space_id, type_id, section_id, name FROM project.control_circuit "
	"WHERE space_id = $1 AND section_id = $2"
	"OFFSET $3 LIMIT $4",
	pg::Query::Name{"select_control_circuits"},
};

const pg::Query kCount{
	"SELECT COUNT(*) FROM project.control_circuit",
	pg::Query::Name{"count_control_circuits"},
};

PagingResult<model::ControlCircuit> ControlCircuit::GetList(const boost::uuids::uuid& spaceId, int64_t sectionId, int start, int limit) {
	PagingResult<model::ControlCircuit> data;

	auto trx = _pg->Begin(pg::Transaction::RO);
	auto res = trx.Execute(kSelectControlCircuits, spaceId, sectionId, start, limit);
	data.items = res.AsContainer<decltype(data.items)>(pg::kRowTag);
	res = trx.Execute(kCount);
	data.total = res.AsSingleRow<int64_t>();
	trx.Commit();
	return data;
}

} // namespace svetit::project::table
