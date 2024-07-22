#include "api/simple_crud.hpp"
#include "api/simple_list.hpp"
#include "api/cc-mode-type-list.hpp"
#include "model/cc_status_category.hpp"
#include "model/cc_status_type.hpp"
#include "model/cc_type.hpp"
#include "model/code.hpp"
#include "model/control_circuit.hpp"
#include "model/device_item.hpp"
#include "model/device.hpp"
#include "model/di_type.hpp"
#include "model/measure.hpp"
#include "model/plugin.hpp"
#include "model/save_timer.hpp"
#include "model/translation.hpp"
#include "model/value_view.hpp"
#include "model/param_type.hpp"
#include "model/section.hpp"
#include "model/cc_di.hpp"
#include "model/project.hpp"
#include "model/cc_param.hpp"
#include "model/cc_type_di_type.hpp"
#include "model/cc_type_param.hpp"
#include "model/device_plugin_param.hpp"
#include "model/di_plugin_param.hpp"
#include "model/section_param.hpp"
#include "model/project_param.hpp"

#include "model/serialize.hpp"

#include "repo/repository.hpp"
#include "service/service.hpp"
#include "api/details.hpp"

#include <userver/components/minimal_server_component_list.hpp>
#include <userver/storages/postgres/component.hpp>
#include <userver/clients/dns/component.hpp>
#include <userver/clients/http/component.hpp>
#include <userver/server/handlers/ping.hpp>
#include <userver/server/handlers/tests_control.hpp>
#include <userver/testsuite/testsuite_support.hpp>
#include <userver/utils/daemon_run.hpp>

namespace svetit::project::handlers {

#define DECLARE_SIMPLE_HANDLER(modelName) \
	using modelName = SimpleCrud<model::modelName>; \
	using modelName ## List = SimpleList<model::modelName>

	DECLARE_SIMPLE_HANDLER(CcDi);
	DECLARE_SIMPLE_HANDLER(CcParam);
	DECLARE_SIMPLE_HANDLER(CcStatusCategory);
	DECLARE_SIMPLE_HANDLER(CcStatusType);
	DECLARE_SIMPLE_HANDLER(CcType);
	DECLARE_SIMPLE_HANDLER(CcTypeDiType);
	DECLARE_SIMPLE_HANDLER(CcTypeParam);
	DECLARE_SIMPLE_HANDLER(Code);
	DECLARE_SIMPLE_HANDLER(ControlCircuit);
	DECLARE_SIMPLE_HANDLER(Device);
	DECLARE_SIMPLE_HANDLER(DeviceItem);
	DECLARE_SIMPLE_HANDLER(DevicePluginParam);
	DECLARE_SIMPLE_HANDLER(DiPluginParam);
	DECLARE_SIMPLE_HANDLER(DiType);
	DECLARE_SIMPLE_HANDLER(Measure);
	DECLARE_SIMPLE_HANDLER(Plugin);
	DECLARE_SIMPLE_HANDLER(ParamType);
	DECLARE_SIMPLE_HANDLER(Project);
	DECLARE_SIMPLE_HANDLER(ProjectParam);
	DECLARE_SIMPLE_HANDLER(SaveTimer);
	DECLARE_SIMPLE_HANDLER(Section);
	DECLARE_SIMPLE_HANDLER(SectionParam);
	DECLARE_SIMPLE_HANDLER(Translation);
	DECLARE_SIMPLE_HANDLER(ValueView);

	using CcModeType = SimpleCrud<model::CcModeType>;
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
