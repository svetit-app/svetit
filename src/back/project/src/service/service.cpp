#include "service.hpp"

#include <boost/uuid/uuid_generators.hpp>

#include "../repo/repository.hpp"

#include <userver/yaml_config/merge_schemas.hpp>
#include <userver/components/component_config.hpp>
#include <userver/components/component_context.hpp>

namespace svetit::project {

/*static*/ yaml_config::Schema Service::GetStaticConfigSchema()
{
	return yaml_config::MergeSchemas<components::LoggableComponentBase>(R"(
type: object
description: Main service component
additionalProperties: false
properties:
  items-limit-for-list:
    type: string
    description: How many items list may contain
)");
}

Service::Service(
		const components::ComponentConfig& conf,
		const components::ComponentContext& ctx)
	: components::LoggableComponentBase{conf, ctx}
	, _repo{ctx.FindComponent<Repository>()}
	, _itemsLimitForList{conf["items-limit-for-list"].As<int>()}
{}


} // namespace svetit::project
