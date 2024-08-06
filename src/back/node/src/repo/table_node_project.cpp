#include "table_node_project.hpp"
#include <memory>
#include <shared/errors.hpp>
#include <shared/paging.hpp>

#include <userver/components/component_config.hpp>
#include <userver/components/component_context.hpp>
#include <userver/utils/boost_uuid4.hpp>
#include <userver/yaml_config/merge_schemas.hpp>
#include <userver/storages/postgres/component.hpp>

namespace svetit::node::table {

namespace pg = storages::postgres;
using pg::ClusterHostType;

NodeProject::NodeProject(std::shared_ptr<db::Base> dbPtr)
	: _db{std::move(dbPtr)}
{}

const pg::Query kSelect {
	"SELECT node_id, project_id FROM node.node_project WHERE node_id = $1 AND project_id = $2",
	pg::Query::Name{"nodeProject.select"},
};

model::NodeProject NodeProject::Select(const boost::uuids::uuid& nodeId, const boost::uuids::uuid& projectId) {
	auto res = _db->Execute(ClusterHostType::kSlave, kSelect, nodeId, projectId);
	if (res.IsEmpty())
		throw errors::NotFound404{};

	return res.AsSingleRow<model::NodeProject>(pg::kRowTag);
}

const pg::Query kDelete {
	"DELETE FROM node.node_project WHERE node_id = $1 AND project_id = $2",
	pg::Query::Name{"nodeProject.delete"},
};

void NodeProject::Delete(const boost::uuids::uuid& nodeId, const boost::uuids::uuid& projectId) {
	auto res = _db->Execute(ClusterHostType::kMaster, kDelete, nodeId, projectId);
	if (!res.RowsAffected())
		throw errors::NotFound404();
}

const pg::Query kInsert {
	"INSERT INTO node.node_project (node_id, project_id) VALUES ($1, $2)",
	pg::Query::Name{"nodeProject.insert"},
};

void NodeProject::Create(const model::NodeProject& item) {
	_db->Execute(ClusterHostType::kMaster, kInsert, item.nodeId, item.projectId);
}

const pg::Query kSelectList {
	"SELECT node_id, project_id, COUNT(*) OVER() FROM node.node_project OFFSET $1 LIMIT $2",
	pg::Query::Name{"nodeProject.select_list"},
};

PagingResult<model::NodeProject> NodeProject::SelectList(int32_t start, int32_t limit) {
	auto res = _db->Execute(ClusterHostType::kSlave, kSelectList, start, limit);
	PagingResult<model::NodeProject> data;
	data = res.AsContainer<decltype(data)::RawContainer>(pg::kRowTag);
	return data;
}

const pg::Query kSelectListByNodeId {
	"SELECT node_id, project_id, COUNT(*) OVER() FROM node.node_project WHERE node_id = $1 OFFSET $2 LIMIT $3",
	pg::Query::Name{"nodeProject.select_list_by_nodeId"},
};

PagingResult<model::NodeProject> NodeProject::SelectListByNodeId(int32_t start, int32_t limit, const boost::uuids::uuid& nodeId) {
	auto res = _db->Execute(ClusterHostType::kSlave, kSelectListByNodeId, nodeId, start, limit);
	PagingResult<model::NodeProject> data;
	data = res.AsContainer<decltype(data)::RawContainer>(pg::kRowTag);
	return data;
}

} // namespace svetit::node::table
