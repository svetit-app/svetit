#pragma once

#include "table_node.hpp"
#include "table_node_project.hpp"

#include <shared/type_utils.hpp>
#include <shared/db/db_base.hpp>

#include <userver/components/loggable_component_base.hpp>
#include <userver/yaml_config/schema.hpp>
#include <userver/utest/using_namespace_userver.hpp>

namespace svetit::node {

class Repository final : public components::LoggableComponentBase {
public:
	static constexpr std::string_view kName = "repository";

	static yaml_config::Schema GetStaticConfigSchema();

	explicit Repository(
		const components::ComponentConfig& conf,
		const components::ComponentContext& ctx);

	table::Node& Node();
	table::NodeProject& NodeProject();

private:
	std::shared_ptr<db::Base> _db;
	table::Node _node;
	table::NodeProject _nodeProject;
};

} // namespace svetit::node
