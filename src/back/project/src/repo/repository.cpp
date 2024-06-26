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
	, _pg{ctx.FindComponent<components::Postgres>("database").GetCluster()}
	, _db{std::make_shared<db::Base>(_pg)}
	, _project{_db}
	, _paramType{_pg}
	, _projectParam{_pg}
	, _section{_pg}
	, _sectionParam{_pg}
	, _ccType{_pg}
	, _controlCircuit{_pg}
	, _plugin{_pg}
	, _device{_pg}
	, _devicePluginParam{_pg}
	, _code{_pg}
	, _measure{_pg}
	, _saveTimer{_pg}
	, _ccTypeParam{_pg}
	, _diType{_pg}
	, _diPluginParam{_pg}
	, _ccTypeDiType{_pg}
	, _deviceItem{_pg}
	, _ccModeType{_db}
	, _ccDi{_db}
	, _ccParam{_pg}
	, _ccStatusCategory{_pg}
	, _ccStatusType{_pg}
	, _valueView{_pg}
	, _translation{_pg}
{}

} // namespace svetit::project
