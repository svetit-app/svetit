#pragma once

#include <userver/components/loggable_component_base.hpp>
#include <userver/yaml_config/schema.hpp>
#include <userver/utest/using_namespace_userver.hpp>
#include <userver/storages/postgres/cluster.hpp>

#include <shared/type_utils.hpp>

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
#include "../model/cc_di.hpp"
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

	template<typename T>
	auto Table();

private:
	storages::postgres::ClusterPtr _pg;
	std::shared_ptr<db::Base> _db;
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
	db::Table<model::CcDi> _ccDi;
	table::CcParam _ccParam;
	table::CcStatusCategory _ccStatusCategory;
	table::CcStatusType _ccStatusType;
	table::ValueView _valueView;
	table::Translation _translation;
};

template<typename T>
inline auto Repository::Table()
{
	if constexpr (std::is_same<T, model::Project>::value)
		return &_project;
	else if constexpr (std::is_same<T, model::ParamType>::value)
		return &_paramType;
	else if constexpr (std::is_same<T, model::ProjectParam>::value)
		return &_projectParam;
	else if constexpr (std::is_same<T, model::Section>::value)
		return &_section;
	else if constexpr (std::is_same<T, model::SectionParam>::value)
		return &_sectionParam;
	else if constexpr (std::is_same<T, model::CcType>::value)
		return &_ccType;
	else if constexpr (std::is_same<T, model::ControlCircuit>::value)
		return &_controlCircuit;
	else if constexpr (std::is_same<T, model::Plugin>::value)
		return &_plugin;
	else if constexpr (std::is_same<T, model::Device>::value)
		return &_device;
	else if constexpr (std::is_same<T, model::DevicePluginParam>::value)
		return &_devicePluginParam;
	else if constexpr (std::is_same<T, model::Code>::value)
		return &_code;
	else if constexpr (std::is_same<T, model::Measure>::value)
		return &_measure;
	else if constexpr (std::is_same<T, model::SaveTimer>::value)
		return &_saveTimer;
	else if constexpr (std::is_same<T, model::CcTypeParam>::value)
		return &_ccTypeParam;
	else if constexpr (std::is_same<T, model::DiType>::value)
		return &_diType;
	else if constexpr (std::is_same<T, model::DiPluginParam>::value)
		return &_diPluginParam;
	else if constexpr (std::is_same<T, model::CcTypeDiType>::value)
		return &_ccTypeDiType;
	else if constexpr (std::is_same<T, model::DeviceItem>::value)
		return &_deviceItem;
	else if constexpr (std::is_same<T, model::CcModeType>::value)
		return &_ccModeType;
	else if constexpr (std::is_same<T, model::CcDi>::value)
		return &_ccDi;
	else if constexpr (std::is_same<T, model::CcParam>::value)
		return &_ccParam;
	else if constexpr (std::is_same<T, model::CcStatusCategory>::value)
		return &_ccStatusCategory;
	else if constexpr (std::is_same<T, model::CcStatusType>::value)
		return &_ccStatusType;
	else if constexpr (std::is_same<T, model::ValueView>::value)
		return &_valueView;
	else if constexpr (std::is_same<T, model::Translation>::value)
		return &_translation;
	else
		static_assert(std::is_same<T, void>::value && "unknown table for type");
}

} // namespace svetit::project
