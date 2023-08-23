#include "repository.hpp"

#include "state.hpp"

#include "userver/components/component_config.hpp"
#include "userver/components/component_context.hpp"
#include <userver/yaml_config/merge_schemas.hpp>
#include <userver/storages/postgres/component.hpp>

namespace svetit::auth {

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
	, _pg{ctx.FindComponent<components::Postgres>("database").GetCluster()},
	_state{svetit::auth::State(_pg)}
{}

State& Repository::State(){
	return _state;
}

} // namespace svetit::auth