#include "table_node_group.hpp"
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

NodeGroup::NodeGroup(std::shared_ptr<db::Base> dbPtr)
	: _db{std::move(dbPtr)}
{}

const pg::Query kSelect {
	"SELECT node_id, group_id FROM node.node_group WHERE node_id = $1 AND group_id = $2",
	pg::Query::Name{"nodeGroup.select"},
};

model::NodeGroup NodeGroup::Select(const boost::uuids::uuid& nodeId, int groupId) {
	auto res = _db->Execute(ClusterHostType::kSlave, kSelect, nodeId, groupId);
	if (res.IsEmpty())
		throw errors::NotFound404{};

	return res.AsSingleRow<model::NodeGroup>(pg::kRowTag);
}

const pg::Query kDelete {
	"DELETE FROM node.node_group WHERE node_id = $1 AND group_id = $2",
	pg::Query::Name{"nodeGroup.delete"},
};

void NodeGroup::Delete(const boost::uuids::uuid& nodeId, int groupId) {
	auto res = _db->Execute(ClusterHostType::kMaster, kDelete, nodeId, groupId);
	if (!res.RowsAffected())
		throw errors::NotFound404();
}

const pg::Query kInsert {
	"INSERT INTO node.node_group (node_id, group_id) VALUES ($1, $2)",
	pg::Query::Name{"nodeGroup.insert"},
};

void NodeGroup::Create(const model::NodeGroup& item) {
	_db->Execute(ClusterHostType::kMaster, kInsert, item.nodeId, item.groupId);
}

const pg::Query kSelectList {
	"SELECT node_id, group_id, COUNT(*) OVER() FROM node.node_group OFFSET $1 LIMIT $2",
	pg::Query::Name{"nodeGroup.select_list"},
};

PagingResult<model::NodeGroup> NodeGroup::SelectList(int32_t start, int32_t limit) {
	auto res = _db->Execute(ClusterHostType::kSlave, kSelectList, start, limit);
	PagingResult<model::NodeGroup> data;
	data = res.AsContainer<decltype(data)::RawContainer>(pg::kRowTag);
	return data;
}

const pg::Query kSelectListByNodeId {
	"SELECT node_id, group_id, COUNT(*) OVER() FROM node.node_group WHERE node_id = $1 OFFSET $2 LIMIT $3",
	pg::Query::Name{"nodeGroup.select_list_by_nodeId"},
};

PagingResult<model::NodeGroup> NodeGroup::SelectListByNodeId(int32_t start, int32_t limit, const boost::uuids::uuid& nodeId) {
	auto res = _db->Execute(ClusterHostType::kSlave, kSelectListByNodeId, nodeId, start, limit);
	PagingResult<model::NodeGroup> data;
	data = res.AsContainer<decltype(data)::RawContainer>(pg::kRowTag);
	return data;
}

} // namespace svetit::node::table
