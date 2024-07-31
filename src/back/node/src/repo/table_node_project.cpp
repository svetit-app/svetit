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

} // namespace svetit::node::table
