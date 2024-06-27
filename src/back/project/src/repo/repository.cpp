#include "repository.hpp"

#include <userver/components/component_config.hpp>
#include <userver/components/component_context.hpp>
#include <userver/yaml_config/merge_schemas.hpp>
#include <userver/storages/postgres/component.hpp>

namespace svetit::project {

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
	, _project{_db}
	, _paramType{_db}
	, _projectParam{_db}
	, _section{_db}
	, _sectionParam{_db}
	, _ccType{_db}
	, _controlCircuit{_db}
	, _plugin{_db}
	, _device{_db}
	, _devicePluginParam{_db}
	, _code{_db}
	, _measure{_db}
	, _saveTimer{_db}
	, _ccTypeParam{_db}
	, _diType{_db}
	, _diPluginParam{_db}
	, _ccTypeDiType{_db}
	, _deviceItem{_db}
	, _ccModeType{_db}
	, _ccDi{_db}
	, _ccParam{_db}
	, _ccStatusCategory{_db}
	, _ccStatusType{_db}
	, _valueView{_db}
	, _translation{_db}
{}

} // namespace svetit::project
