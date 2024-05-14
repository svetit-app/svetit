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

	template<typename T, typename... Args>
	T Get(Args&&... args);

	template<typename T, typename R = decltype(T::id)>
	R Create(const T& arg);
	
	template<typename T>
	void Update(const T& arg);
	
	template<typename T, typename... Args>
	void Delete(Args&&... args);

private:
	template<typename T>
	auto getSimpleTable();

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

template<typename T, typename... Args>
inline T Repository::Get(Args&&... args)
{
	return getSimpleTable<T>()->Get(std::forward<Args>(args)...);
}

template<typename T, typename R>
inline R Repository::Create(const T& arg)
{
	return getSimpleTable<T>()->Create(arg);
}

template<typename T>
inline void Repository::Update(const T& arg)
{
	getSimpleTable<T>()->Update(arg);
}

template<typename T, typename... Args>
inline void Repository::Delete(Args&&... args)
{
	getSimpleTable<T>()->Delete(std::forward<Args>(args)...);
}

template<typename T>
inline auto Repository::getSimpleTable()
{
	if constexpr (std::is_same<T, ReturnType<decltype(&table::Project::Get)>::type>::value)
		return &_project;
	else if constexpr (std::is_same<T, ReturnType<decltype(&table::ParamType::Get)>::type>::value)
		return &_paramType;
	else if constexpr (std::is_same<T, ReturnType<decltype(&table::ProjectParam::Get)>::type>::value)
		return &_projectParam;
	else if constexpr (std::is_same<T, ReturnType<decltype(&table::Section::Get)>::type>::value)
		return &_section;
	else if constexpr (std::is_same<T, ReturnType<decltype(&table::SectionParam::Get)>::type>::value)
		return &_sectionParam;
	else if constexpr (std::is_same<T, ReturnType<decltype(&table::CcType::Get)>::type>::value)
		return &_ccType;
	else if constexpr (std::is_same<T, ReturnType<decltype(&table::ControlCircuit::Get)>::type>::value)
		return &_controlCircuit;
	else if constexpr (std::is_same<T, ReturnType<decltype(&table::Plugin::Get)>::type>::value)
		return &_plugin;
	else if constexpr (std::is_same<T, ReturnType<decltype(&table::Device::Get)>::type>::value)
		return &_device;
	else if constexpr (std::is_same<T, ReturnType<decltype(&table::DevicePluginParam::Get)>::type>::value)
		return &_devicePluginParam;
	else if constexpr (std::is_same<T, ReturnType<decltype(&table::Code::Get)>::type>::value)
		return &_code;
	else if constexpr (std::is_same<T, ReturnType<decltype(&table::Measure::Get)>::type>::value)
		return &_measure;
	else if constexpr (std::is_same<T, ReturnType<decltype(&table::SaveTimer::Get)>::type>::value)
		return &_saveTimer;
	else if constexpr (std::is_same<T, ReturnType<decltype(&table::CcTypeParam::Get)>::type>::value)
		return &_ccTypeParam;
	else if constexpr (std::is_same<T, ReturnType<decltype(&table::DiType::Get)>::type>::value)
		return &_diType;
	else if constexpr (std::is_same<T, ReturnType<decltype(&table::DiPluginParam::Get)>::type>::value)
		return &_diPluginParam;
	else if constexpr (std::is_same<T, ReturnType<decltype(&table::CcTypeDiType::Get)>::type>::value)
		return &_ccTypeDiType;
	else if constexpr (std::is_same<T, ReturnType<decltype(&table::DeviceItem::Get)>::type>::value)
		return &_deviceItem;
	else if constexpr (std::is_same<T, ReturnType<decltype(&table::CcModeType::Get)>::type>::value)
		return &_ccModeType;
	else if constexpr (std::is_same<T, ReturnType<decltype(&table::CcDi::Get)>::type>::value)
		return &_ccDi;
	else if constexpr (std::is_same<T, ReturnType<decltype(&table::CcParam::Get)>::type>::value)
		return &_ccParam;
	else if constexpr (std::is_same<T, ReturnType<decltype(&table::CcStatusCategory::Get)>::type>::value)
		return &_ccStatusCategory;
	else if constexpr (std::is_same<T, ReturnType<decltype(&table::CcStatusType::Get)>::type>::value)
		return &_ccStatusType;
	else if constexpr (std::is_same<T, ReturnType<decltype(&table::ValueView::Get)>::type>::value)
		return &_valueView;
	else if constexpr (std::is_same<T, ReturnType<decltype(&table::Translation::Get)>::type>::value)
		return &_translation;
	else
		static_assert(std::is_same<T, void>::value && "unknown table for type");
}

} // namespace svetit::project
