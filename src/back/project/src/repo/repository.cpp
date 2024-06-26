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
	, _ccModeType{_pg}
	, _ccDi{_pg}
	, _ccParam{_pg}
	, _ccStatusCategory{_pg}
	, _ccStatusType{_pg}
	, _valueView{_pg}
	, _translation{_pg}
{}

table::Project& Repository::Project() {
	return _project;
}

table::ParamType& Repository::ParamType() {
	return _paramType;
}

table::ProjectParam& Repository::ProjectParam() {
	return _projectParam;
}

table::Section& Repository::Section() {
	return _section;
}

table::SectionParam& Repository::SectionParam() {
	return _sectionParam;
}

table::CcType& Repository::CcType() {
	return _ccType;
}

table::ControlCircuit& Repository::ControlCircuit() {
	return _controlCircuit;
}

table::Plugin& Repository::Plugin() {
	return _plugin;
}

table::Device& Repository::Device() {
	return _device;
}

table::DevicePluginParam& Repository::DevicePluginParam() {
	return _devicePluginParam;
}

table::Code& Repository::Code() {
	return _code;
}

table::Measure& Repository::Measure() {
	return _measure;
}

table::SaveTimer& Repository::SaveTimer() {
	return _saveTimer;
}

table::CcTypeParam& Repository::CcTypeParam() {
	return _ccTypeParam;
}

table::DiType& Repository::DiType() {
	return _diType;
}

table::DiPluginParam& Repository::DiPluginParam() {
	return _diPluginParam;
}

table::CcTypeDiType& Repository::CcTypeDiType() {
	return _ccTypeDiType;
}

table::DeviceItem& Repository::DeviceItem() {
	return _deviceItem;
}

table::CcModeType& Repository::CcModeType() {
	return _ccModeType;
}

table::CcDi& Repository::CcDi() {
	return _ccDi;
}

table::CcParam& Repository::CcParam() {
	return _ccParam;
}

table::CcStatusCategory& Repository::CcStatusCategory() {
	return _ccStatusCategory;
}

table::CcStatusType& Repository::CcStatusType() {
	return _ccStatusType;
}

table::ValueView& Repository::ValueView() {
	return _valueView;
}

table::Translation& Repository::Translation() {
	return _translation;
}

} // namespace svetit::project
