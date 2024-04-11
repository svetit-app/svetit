#pragma once

#include <userver/components/loggable_component_base.hpp>
#include <userver/yaml_config/schema.hpp>
#include <userver/utest/using_namespace_userver.hpp>
#include <userver/storages/postgres/cluster.hpp>

#include "table_project.hpp"
#include "table_param_type.hpp"
#include "table_project_param.hpp"
#include "table_section.hpp"
#include "table_section_param.hpp"
#include "table_cc_type.hpp"
#include "table_control_circuit.hpp"
#include "table_plugin.hpp"
#include "table_device.hpp"
#include "table_device_plugin_param.hpp"
#include "table_code.hpp"
#include "table_measure.hpp"
#include "table_save_timer.hpp"
#include "table_cc_type_param.hpp"
#include "table_di_type.hpp"
#include "table_di_plugin_param.hpp"
#include "table_cc_type_di_type.hpp"
#include "table_device_item.hpp"
#include "table_cc_mode_type.hpp"
#include "table_cc_di.hpp"
#include "table_cc_param.hpp"
#include "table_cc_status_category.hpp"
#include "table_cc_status_type.hpp"
#include "table_value_view.hpp"
#include "table_translation.hpp"

namespace svetit::project {

class Repository final : public components::LoggableComponentBase {
public:
	static constexpr std::string_view kName = "repository";

	static yaml_config::Schema GetStaticConfigSchema();

	explicit Repository(
		const components::ComponentConfig& conf,
		const components::ComponentContext& ctx);

	table::Project& Project();
	table::ParamType& ParamType();
	table::ProjectParam& ProjectParam();
	table::Section& Section();
	table::SectionParam& SectionParam();
	table::CcType& CcType();
	table::ControlCircuit& ControlCircuit();
	table::Plugin& Plugin();
	table::Device& Device();
	table::DevicePluginParam& DevicePluginParam();
	table::Code& Code();
	table::Measure& Measure();
	table::SaveTimer& SaveTimer();
	table::CcTypeParam& CcTypeParam();
	table::DiType& DiType();
	table::DiPluginParam& DiPluginParam();
	table::CcTypeDiType& CcTypeDiType();
	table::DeviceItem& DeviceItem();
	table::CcModeType& CcModeType();
	table::CcDi& CcDi();
	table::CcParam& CcParam();
	table::CcStatusCategory& CcStatusCategory();
	table::CcStatusType& CcStatusType();
	table::ValueView& ValueView();
	table::Translation& Translation();

private:
	storages::postgres::ClusterPtr _pg;
	table::Project _project;
	table::ParamType _paramType;
	table::ProjectParam _projectParam;
	table::Section _section;
	table::SectionParam _sectionParam;
	table::CcType _ccType;
	table::ControlCircuit _controlCircuit;
	table::Plugin _plugin;
	table::Device _device;
	table::DevicePluginParam _devicePluginParam;
	table::Code _code;
	table::Measure _measure;
	table::SaveTimer _saveTimer;
	table::CcTypeParam _ccTypeParam;
	table::DiType _diType;
	table::DiPluginParam _diPluginParam;
	table::CcTypeDiType _ccTypeDiType;
	table::DeviceItem _deviceItem;
	table::CcModeType _ccModeType;
	table::CcDi _ccDi;
	table::CcParam _ccParam;
	table::CcStatusCategory _ccStatusCategory;
	table::CcStatusType _ccStatusType;
	table::ValueView _valueView;
	table::Translation _translation;
};

} // namespace svetit::project
