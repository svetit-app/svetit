#pragma once

#include <userver/components/loggable_component_base.hpp>
#include <userver/yaml_config/schema.hpp>
#include <userver/utest/using_namespace_userver.hpp>

#include <shared/type_utils.hpp>

#include "table_project.hpp"
#include "table_cc_mode_type.hpp"
#include "../model/param_type.hpp"
#include "../model/project_param.hpp"
#include "../model/section.hpp"
#include "../model/section_param.hpp"
#include "../model/cc_type.hpp"
#include "../model/control_circuit.hpp"
#include "../model/plugin.hpp"
#include "../model/device.hpp"
#include "../model/device_plugin_param.hpp"
#include "../model/code.hpp"
#include "../model/measure.hpp"
#include "../model/save_timer.hpp"
#include "../model/cc_type_param.hpp"
#include "../model/di_type.hpp"
#include "../model/di_plugin_param.hpp"
#include "../model/cc_type_di_type.hpp"
#include "../model/device_item.hpp"
#include "../model/cc_di.hpp"
#include "../model/cc_param.hpp"
#include "../model/cc_status_category.hpp"
#include "../model/cc_status_type.hpp"
#include "../model/value_view.hpp"
#include "../model/translation.hpp"

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
	std::shared_ptr<db::Base> _db;
	table::Project _project;
	db::Table<model::ParamType> _paramType;
	db::Table<model::ProjectParam> _projectParam;
	db::Table<model::Section> _section;
	db::Table<model::SectionParam> _sectionParam;
	db::Table<model::CcType> _ccType;
	db::Table<model::ControlCircuit> _controlCircuit;
	db::Table<model::Plugin> _plugin;
	db::Table<model::Device> _device;
	db::Table<model::DevicePluginParam> _devicePluginParam;
	db::Table<model::Code> _code;
	db::Table<model::Measure> _measure;
	db::Table<model::SaveTimer> _saveTimer;
	db::Table<model::CcTypeParam> _ccTypeParam;
	db::Table<model::DiType> _diType;
	db::Table<model::DiPluginParam> _diPluginParam;
	db::Table<model::CcTypeDiType> _ccTypeDiType;
	db::Table<model::DeviceItem> _deviceItem;
	table::CcModeType _ccModeType;
	db::Table<model::CcDi> _ccDi;
	db::Table<model::CcParam> _ccParam;
	db::Table<model::CcStatusCategory> _ccStatusCategory;
	db::Table<model::CcStatusType> _ccStatusType;
	db::Table<model::ValueView> _valueView;
	db::Table<model::Translation> _translation;
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
