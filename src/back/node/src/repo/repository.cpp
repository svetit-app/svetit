#include "repository.hpp"

#include "table_node.hpp"
#include "table_node_project.hpp"

#include <userver/components/component_config.hpp>
#include <userver/components/component_context.hpp>
#include <userver/yaml_config/merge_schemas.hpp>
#include <userver/storages/postgres/component.hpp>
#include <userver/utest/using_namespace_userver.hpp>

#include <shared/type_utils.hpp>

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
{}

table::Node& Repository::Node() {
	return _node;
}

table::NodeProject& Repository::NodeProject() {
	return _nodeProject;
}

} // namespace svetit::node
