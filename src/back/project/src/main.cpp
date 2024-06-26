#include <userver/components/minimal_server_component_list.hpp>
#include <userver/storages/postgres/component.hpp>
#include <userver/clients/dns/component.hpp>
#include <userver/clients/http/component.hpp>
#include <userver/server/handlers/ping.hpp>
#include <userver/server/handlers/tests_control.hpp>
#include <userver/testsuite/testsuite_support.hpp>
#include <userver/utils/daemon_run.hpp>

#include <shared/simple_crud.hpp>
#include <shared/simple_list.hpp>
#include "api/simple_crud.hpp"
#include "api/simple_list.hpp"
#include "api/cc-mode-type-list.hpp"
#include "model/cc_status_category_serialize.hpp"
#include "model/cc_status_type_serialize.hpp"
#include "model/cc_type_serialize.hpp"
#include "model/code_serialize.hpp"
#include "model/control_circuit_serialize.hpp"
#include "model/device_item_serialize.hpp"
#include "model/device_serialize.hpp"
#include "model/di_type_serialize.hpp"
#include "model/measure_serialize.hpp"
#include "model/plugin_serialize.hpp"
#include "model/save_timer_serialize.hpp"
#include "model/translation_serialize.hpp"
#include "model/value_view_serialize.hpp"
#include "model/param_type_serialize.hpp"
#include "model/section_serialize.hpp"
#include "model/cc_di_serialize.hpp"
#include "model/project_serialize.hpp"
#include "model/cc_param_serialize.hpp"
#include "model/cc_type_di_type_serialize.hpp"
#include "model/cc_type_param_serialize.hpp"
#include "model/device_plugin_param_serialize.hpp"
#include "model/di_plugin_param_serialize.hpp"
#include "model/section_param_serialize.hpp"
#include "model/project_param_serialize.hpp"

#include "repo/repository.hpp"
#include "service/service.hpp"
#include "api/details.hpp"

namespace svetit::project::handlers {

#define DECLARE_SIMPLE_HANDLER(modelName, handlerName, idKey) \
	static constexpr std::string_view modelName ## Name = #handlerName; \
	using modelName = SimpleCrud<Service, model::modelName, &modelName ## Name, &idKey>

#define DECLARE_SIMPLE_LIST_HANDLER(modelName, handlerName, listFilterKey) \
	static constexpr std::string_view modelName ## ListName = #handlerName "-list"; \
	static constexpr std::string_view modelName ## ListFilterKey = #listFilterKey; \
	using modelName ## List = SimpleList<Service, model::modelName, &modelName ## ListName, &modelName ## ListFilterKey>

#define DECLARE_SIMPLE_HANDLER_FULL2(modelName, handlerName, listFilterKey) \
	DECLARE_SIMPLE_LIST_HANDLER(modelName, handlerName, listFilterKey); \
	DECLARE_SIMPLE_HANDLER(modelName, handlerName, modelName ## ListFilterKey)

#define DECLARE_SIMPLE_HANDLER_FULL(modelName, handlerName, listFilterKey) \
	DECLARE_SIMPLE_LIST_HANDLER(modelName, handlerName, listFilterKey); \
	DECLARE_SIMPLE_HANDLER(modelName, handlerName, kIdKey)

// Result of DECLARE_SIMPLE_HANDLER_FULL(DeviceItem, handler-device-item, deviceId); is:
//
// extern char const DeviceItemListName[] = "handler-device-item-list";
// extern char const DeviceItemListFilterKey[] = "deviceId";
// using DeviceItemList = SimpleList<Service, model::DeviceItem, DeviceItemListName, DeviceItemListFilterKey>;
// extern char const DeviceItemName[] = "handler-device-item";
// using DeviceItem = SimpleCrud<Service, model::DeviceItem, DeviceItemName, kIdKey>;

	DECLARE_SIMPLE_HANDLER_FULL2(CcDi, handler-cc-di, ccId);
	DECLARE_SIMPLE_HANDLER_FULL2(CcParam, handler-cc-param, ccId);
	DECLARE_SIMPLE_HANDLER_FULL(CcStatusCategory, handler-cc-status-category, projectId);
	DECLARE_SIMPLE_HANDLER_FULL(CcStatusType, handler-cc-status-type, ccTypeId);
	DECLARE_SIMPLE_HANDLER_FULL(CcType, handler-cc-type, projectId);
	DECLARE_SIMPLE_HANDLER_FULL2(CcTypeDiType, handler-cc-type-di-type, ccTypeId);
	DECLARE_SIMPLE_HANDLER_FULL2(CcTypeParam, handler-cc-type-param, ccTypeId);
	DECLARE_SIMPLE_HANDLER_FULL(Code, handler-code, projectId);
	DECLARE_SIMPLE_HANDLER_FULL(ControlCircuit, handler-control-circuit, sectionId);
	DECLARE_SIMPLE_HANDLER_FULL(Device, handler-device, projectId);
	DECLARE_SIMPLE_HANDLER_FULL(DeviceItem, handler-device-item, deviceId);
	DECLARE_SIMPLE_HANDLER_FULL2(DevicePluginParam, handler-device-plugin-param, deviceId);
	DECLARE_SIMPLE_HANDLER_FULL2(DiPluginParam, handler-di-plugin-param, diTypeId);
	DECLARE_SIMPLE_HANDLER_FULL(DiType, handler-di-type, projectId);
	DECLARE_SIMPLE_HANDLER_FULL(Measure, handler-measure, projectId);
	DECLARE_SIMPLE_HANDLER_FULL(Plugin, handler-plugin, projectId);
	DECLARE_SIMPLE_HANDLER_FULL(ParamType, handler-param-type, projectId);
	DECLARE_SIMPLE_HANDLER_FULL2(ProjectParam, handler-project-param, projectId);
	DECLARE_SIMPLE_HANDLER_FULL(SaveTimer, handler-save-timer, projectId);
	DECLARE_SIMPLE_HANDLER_FULL(Section, handler-section, projectId);
	DECLARE_SIMPLE_HANDLER_FULL2(SectionParam, handler-section-param, sectionId);
	DECLARE_SIMPLE_HANDLER_FULL(Translation, handler-translation, projectId);
	DECLARE_SIMPLE_HANDLER_FULL(ValueView, handler-value-view, diTypeId);
	DECLARE_SIMPLE_HANDLER(CcModeType, handler-cc-mode-type, kIdKey);

	namespace h = svetit::handlers;
	using ProjectList = h::SimpleList2<Service, model::Project>;
	using Project = h::SimpleCrud2<Service, model::Project>;
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
