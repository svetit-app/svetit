#include "service.hpp"
#include "../repo/repository.hpp"

#include <boost/uuid/uuid_generators.hpp>
#include <boost/crc.hpp>

#include <svetit/errors.hpp>
#include <svetit/paging.hpp>
#include <svetit/time_utils.hpp>

#include <userver/yaml_config/merge_schemas.hpp>
#include <userver/components/component_config.hpp>
#include <userver/components/component_context.hpp>

namespace svetit::dtlsgateway {

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
  json-schemas-path:
    type: string
    description: Path to folder with JSON schemas
)");
}

Service::Service(
		const components::ComponentConfig& conf,
		const components::ComponentContext& ctx)
	: components::LoggableComponentBase{conf, ctx}
	, _repo{ctx.FindComponent<RepositoryComponent>()}
	, _itemsLimitForList{conf["items-limit-for-list"].As<int>()}
	, _jsonSchemasPath{conf["json-schemas-path"].As<std::string>()}
{
}

RepositoryComponent& Service::Repo() {
	return _repo;
}

const std::string& Service::GetJSONSchemasPath() {
	return _jsonSchemasPath;
}

} // namespace svetit::dtlsgateway
