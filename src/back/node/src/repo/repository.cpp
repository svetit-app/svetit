#include "repository.hpp"

#include "table_node.hpp"
#include "table_node_project.hpp"
#include "table_node_group.hpp"
#include "table_group_user.hpp"
#include "../model/node.hpp"

#include <userver/components/component_config.hpp>
#include <userver/components/component_context.hpp>
#include <userver/yaml_config/merge_schemas.hpp>
#include <userver/storages/postgres/component.hpp>
#include <userver/utest/using_namespace_userver.hpp>
#include <boost/uuid/uuid_io.hpp>

#include <shared/type_utils.hpp>
#include <shared/paging.hpp>
#include <shared/db/db_base.hpp>

namespace svetit::node {

namespace pg = storages::postgres;
using pg::ClusterHostType;

/*static*/ yaml_config::Schema Repository::GetStaticConfigSchema()
{
	return yaml_config::MergeSchemas<components::LoggableComponentBase>(R"(
type: object
description: Main repository component
additionalProperties: false
properties:
  some-var:
    type: string
    description: some desc
)");
}

Repository::Repository(
		const components::ComponentConfig& conf,
		const components::ComponentContext& ctx)
	: components::LoggableComponentBase{conf, ctx}
	, _db{std::make_shared<db::Base>(ctx.FindComponent<components::Postgres>("database").GetCluster())}
	, _node{_db}
	, _nodeProject{_db}
	, _nodeGroup{_db}
	, _groupUser{_db}
{}

table::Node& Repository::Node() {
	return _node;
}

table::NodeProject& Repository::NodeProject() {
	return _nodeProject;
}

table::NodeGroup& Repository::NodeGroup() {
	return _nodeGroup;
}

table::GroupUser& Repository::GroupUser() {
	return _groupUser;
}

const pg::Query kSelectNodeList{
	R"~(
		SELECT n.id, n.space_id, n.name, n.description, n.latitude, n.longitude, n.created_at, COUNT(*) OVER()
		FROM node.node n
		LEFT JOIN node.node_group ng ON n.id = ng.node_id
		LEFT JOIN node.group_user gu ON ng.group_id = gu.group_id
		WHERE n.space_id = $1 AND gu.user_id = $2
		OFFSET $3 LIMIT $4;
	)~",
	pg::Query::Name{"select_nodes_for_user_and_space_id"},
};

PagingResult<model::Node> Repository::SelectNodeList(const boost::uuids::uuid& spaceId, const std::string& userId, int32_t start, int32_t limit) {
	auto res = _db->Execute(ClusterHostType::kSlave, kSelectNodeList, spaceId, userId, start, limit);
	PagingResult<model::Node> data;
	data = res.AsContainer<decltype(data)::RawContainer>(pg::kRowTag);
	return data;
}

} // namespace svetit::node
