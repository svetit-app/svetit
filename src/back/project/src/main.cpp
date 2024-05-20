#include <userver/components/minimal_server_component_list.hpp>
#include <userver/storages/postgres/component.hpp>
#include <userver/clients/dns/component.hpp>
#include <userver/clients/http/component.hpp>
#include <userver/server/handlers/ping.hpp>
#include <userver/server/handlers/tests_control.hpp>
#include <userver/testsuite/testsuite_support.hpp>
#include <userver/utils/daemon_run.hpp>

#include "api/simple_crud.hpp"
#include "model/cc_mode_type.hpp"
#include "model/cc_mode_type_serialize.hpp"
#include "model/cc_status_category.hpp"
#include "model/cc_status_category_serialize.hpp"
#include "model/cc_status_type.hpp"
#include "model/cc_status_type_serialize.hpp"
#include "model/cc_type.hpp"
#include "model/cc_type_serialize.hpp"
#include "model/code.hpp"
#include "model/code_serialize.hpp"
#include "model/control_circuit.hpp"
#include "model/control_circuit_serialize.hpp"
#include "model/device_item.hpp"
#include "model/device_item_serialize.hpp"
#include "model/device.hpp"
#include "model/device_serialize.hpp"
#include "model/di_type.hpp"
#include "model/di_type_serialize.hpp"
#include "model/measure.hpp"
#include "model/measure_serialize.hpp"
#include "model/plugin.hpp"
#include "model/plugin_serialize.hpp"
#include "model/save_timer.hpp"
#include "model/save_timer_serialize.hpp"
#include "model/translation.hpp"
#include "model/translation_serialize.hpp"
#include "model/value_view.hpp"
#include "model/value_view_serialize.hpp"
#include "model/param_type_serialize.hpp"
#include "model/section.hpp"
#include "model/section_serialize.hpp"

#include "repo/repository.hpp"
#include "service/service.hpp"
#include "api/details.hpp"
#include "api/project.hpp"
#include "api/project-list.hpp"
#include "api/param-type-list.hpp"
#include "api/project-param.hpp"
#include "api/project-param-list.hpp"
#include "api/section-list.hpp"
#include "api/section-param.hpp"
#include "api/section-param-list.hpp"
#include "api/cc-type-list.hpp"
#include "api/control-circuit-list.hpp"
#include "api/plugin-list.hpp"
#include "api/device-list.hpp"
#include "api/device-plugin-param.hpp"
#include "api/device-plugin-param-list.hpp"
#include "api/code-list.hpp"
#include "api/measure-list.hpp"
#include "api/save-timer-list.hpp"
#include "api/cc-type-param.hpp"
#include "api/cc-type-param-list.hpp"
#include "api/di-type-list.hpp"
#include "api/di-plugin-param.hpp"
#include "api/di-plugin-param-list.hpp"
#include "api/cc-type-di-type.hpp"
#include "api/cc-type-di-type-list.hpp"
#include "api/device-item-list.hpp"
#include "api/cc-mode-type-list.hpp"
#include "api/cc-di.hpp"
#include "api/cc-di-list.hpp"
#include "api/cc-param.hpp"
#include "api/cc-param-list.hpp"
#include "api/cc-status-category-list.hpp"
#include "api/cc-status-type-list.hpp"
#include "api/value-view-list.hpp"
#include "api/translation-list.hpp"

namespace svetit::project::handlers {
	extern char const ccModeTypeName[] = "handler-cc-mode-type";
	using CcModeType = SimpleCrud<Service, model::CcModeType, ccModeTypeName>;

	extern char const ccStatusCategoryName[] = "handler-cc-status-category";
	using CcStatusCategory = SimpleCrud<Service, model::CcStatusCategory, ccStatusCategoryName>;

	extern char const ccStatusTypeName[] = "handler-cc-status-type";
	using CcStatusType = SimpleCrud<Service, model::CcStatusType, ccStatusTypeName>;

	extern char const ccTypeName[] = "handler-cc-type";
	using CcType = SimpleCrud<Service, model::CcType, ccTypeName>;

	extern char const codeName[] = "handler-code";
	using Code = SimpleCrud<Service, model::Code, codeName>;

	extern char const controlCircuitName[] = "handler-control-circuit";
	using ControlCircuit = SimpleCrud<Service, model::ControlCircuit, controlCircuitName>;

	extern char const deviceItemName[] = "handler-device-item";
	using DeviceItem = SimpleCrud<Service, model::DeviceItem, deviceItemName>;

	extern char const deviceName[] = "handler-device";
	using Device = SimpleCrud<Service, model::Device, deviceName>;

	extern char const diTypeName[] = "handler-di-type";
	using DiType = SimpleCrud<Service, model::DiType, diTypeName>;

	extern char const measureName[] = "handler-measure";
	using Measure = SimpleCrud<Service, model::Measure, measureName>;

	extern char const pluginName[] = "handler-plugin";
	using Plugin = SimpleCrud<Service, model::Plugin, pluginName>;

	extern char const saveTimerName[] = "handler-save-timer";
	using SaveTimer = SimpleCrud<Service, model::SaveTimer, saveTimerName>;

	extern char const translationName[] = "handler-translation";
	using Translation = SimpleCrud<Service, model::Translation, translationName>;

	extern char const valueViewName[] = "handler-value-view";
	using ValueView = SimpleCrud<Service, model::ValueView, valueViewName>;

	extern char const paramTypeName[] = "handler-param-type";
	using ParamType = SimpleCrud<Service, model::ParamType, paramTypeName>;
	
	extern char const sectionName[] = "handler-section";
	using Section = SimpleCrud<Service, model::Section, sectionName>;
} // namespace svetit::handlers

using namespace svetit::project;

int main(int argc, char* argv[]) {
	auto component_list = components::MinimalServerComponentList()
		.Append<server::handlers::Ping>()
		.Append<components::TestsuiteSupport>()
		.Append<components::HttpClient>()
		.Append<components::Postgres>("database")
		.Append<clients::dns::Component>()
		.Append<server::handlers::TestsControl>()
		.Append<Repository>()
		.Append<Service>()
		.Append<handlers::Details>()
		.Append<handlers::Project>()
		.Append<handlers::ProjectList>()
		.Append<handlers::ParamType>()
		.Append<handlers::ParamTypeList>()
		.Append<handlers::ProjectParam>()
		.Append<handlers::ProjectParamList>()
		.Append<handlers::Section>()
		.Append<handlers::SectionList>()
		.Append<handlers::SectionParam>()
		.Append<handlers::SectionParamList>()
		.Append<handlers::CcType>()
		.Append<handlers::CcTypeList>()
		.Append<handlers::ControlCircuit>()
		.Append<handlers::ControlCircuitList>()
		.Append<handlers::Plugin>()
		.Append<handlers::PluginList>()
		.Append<handlers::Device>()
		.Append<handlers::DeviceList>()
		.Append<handlers::DevicePluginParam>()
		.Append<handlers::DevicePluginParamList>()
		.Append<handlers::Code>()
		.Append<handlers::CodeList>()
		.Append<handlers::Measure>()
		.Append<handlers::MeasureList>()
		.Append<handlers::SaveTimer>()
		.Append<handlers::SaveTimerList>()
		.Append<handlers::CcTypeParam>()
		.Append<handlers::CcTypeParamList>()
		.Append<handlers::DiType>()
		.Append<handlers::DiTypeList>()
		.Append<handlers::DiPluginParam>()
		.Append<handlers::DiPluginParamList>()
		.Append<handlers::CcTypeDiType>()
		.Append<handlers::CcTypeDiTypeList>()
		.Append<handlers::DeviceItem>()
		.Append<handlers::DeviceItemList>()
		.Append<handlers::CcModeType>()
		.Append<handlers::CcModeTypeList>()
		.Append<handlers::CcDi>()
		.Append<handlers::CcDiList>()
		.Append<handlers::CcParam>()
		.Append<handlers::CcParamList>()
		.Append<handlers::CcStatusCategory>()
		.Append<handlers::CcStatusCategoryList>()
		.Append<handlers::CcStatusType>()
		.Append<handlers::CcStatusTypeList>()
		.Append<handlers::ValueView>()
		.Append<handlers::ValueViewList>()
		.Append<handlers::Translation>()
		.Append<handlers::TranslationList>()
		;

	return utils::DaemonMain(argc, argv, component_list);
}
