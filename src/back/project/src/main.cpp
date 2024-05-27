#include <userver/components/minimal_server_component_list.hpp>
#include <userver/storages/postgres/component.hpp>
#include <userver/clients/dns/component.hpp>
#include <userver/clients/http/component.hpp>
#include <userver/server/handlers/ping.hpp>
#include <userver/server/handlers/tests_control.hpp>
#include <userver/testsuite/testsuite_support.hpp>
#include <userver/utils/daemon_run.hpp>

#include "api/simple_crud.hpp"
#include "api/simple_list.hpp"
#include "model/cc_mode_type_serialize.hpp"
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

#include "repo/repository.hpp"
#include "service/service.hpp"
#include "api/details.hpp"
#include "api/cc-type-di-type-list.hpp"
#include "api/cc-type-di-type.hpp"
#include "api/cc-type-param-list.hpp"
#include "api/cc-type-param.hpp"
#include "api/device-plugin-param-list.hpp"
#include "api/device-plugin-param.hpp"
#include "api/di-plugin-param-list.hpp"
#include "api/di-plugin-param.hpp"
#include "api/project-param-list.hpp"
#include "api/project-param.hpp"
#include "api/section-param-list.hpp"
#include "api/section-param.hpp"

namespace svetit::project::handlers {

#define DECLARE_SIMPLE_HANDLER(modelName, handlerName, idKey) \
	extern char const modelName ## Name[] = #handlerName; \
	using modelName = SimpleCrud<Service, model::modelName, modelName ## Name, idKey>

#define DECLARE_SIMPLE_LIST_HANDLER(modelName, handlerName, listFilterKey) \
	extern char const modelName ## ListName[] = #handlerName "-list"; \
	extern char const modelName ## ListFilterKey[] = #listFilterKey; \
	using modelName ## List = SimpleList<Service, model::modelName, modelName ## ListName, modelName ## ListFilterKey>

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

	// TODO: uncomment
	DECLARE_SIMPLE_HANDLER_FULL2(CcDi, handler-cc-di, ccId);
	DECLARE_SIMPLE_HANDLER_FULL2(CcParam, handler-cc-param, ccId);
	DECLARE_SIMPLE_HANDLER_FULL(CcStatusCategory, handler-cc-status-category, projectId);
	DECLARE_SIMPLE_HANDLER_FULL(CcStatusType, handler-cc-status-type, ccTypeId);
	DECLARE_SIMPLE_HANDLER_FULL(CcType, handler-cc-type, projectId);
	// DECLARE_SIMPLE_HANDLER_FULL(CcTypeDiType, handler-cc-type-di_type, ccTypeId);
	// DECLARE_SIMPLE_HANDLER_FULL(CcTypeParam, handler-cc-type-param, ccTypeId);
	DECLARE_SIMPLE_HANDLER_FULL(Code, handler-code, projectId);
	DECLARE_SIMPLE_HANDLER_FULL(ControlCircuit, handler-control-circuit, sectionId);
	DECLARE_SIMPLE_HANDLER_FULL(Device, handler-device, projectId);
	DECLARE_SIMPLE_HANDLER_FULL(DeviceItem, handler-device-item, deviceId);
	DECLARE_SIMPLE_HANDLER_FULL(CcModeType, handler-cc-mode-type, ccTypeId);
	// DECLARE_SIMPLE_HANDLER_FULL(DevicePluginParam, handler-device-plugin-param, deviceId);
	// DECLARE_SIMPLE_HANDLER_FULL(DiPluginParam, handler-di-plugin-param, diTypeId);
	DECLARE_SIMPLE_HANDLER_FULL(DiType, handler-di-type, projectId);
	DECLARE_SIMPLE_HANDLER_FULL(Measure, handler-measure, projectId);
	DECLARE_SIMPLE_HANDLER_FULL(Plugin, handler-plugin, projectId);

	DECLARE_SIMPLE_HANDLER(Project, handler-project, kIdKey);
	extern char const ProjectListName[] = "handler-project-list";
	using ProjectList = SimpleList<Service, model::Project, ProjectListName, nullptr>;

	DECLARE_SIMPLE_HANDLER_FULL(ParamType, handler-param-type, projectId);

	// DECLARE_SIMPLE_HANDLER_FULL(ProjectParam, handler-project-param, projectId);
	DECLARE_SIMPLE_HANDLER_FULL(SaveTimer, handler-save-timer, projectId);
	DECLARE_SIMPLE_HANDLER_FULL(Section, handler-section, projectId);
	// DECLARE_SIMPLE_HANDLER_FULL(SectionParam, handler-section-param, sectionId);
	DECLARE_SIMPLE_HANDLER_FULL(Translation, handler-translation, projectId);
	DECLARE_SIMPLE_HANDLER_FULL(ValueView, handler-value-view, diTypeId);
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
