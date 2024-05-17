#pragma once

#include "../model/project.hpp"
#include "../model/project_param.hpp"
#include "../model/section.hpp"
#include "../model/param_type.hpp"
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
#include "../model/cc_mode_type.hpp"
#include "../model/cc_di.hpp"
#include "../model/cc_param.hpp"
#include "../model/cc_status_category.hpp"
#include "../model/cc_status_type.hpp"
#include "../model/value_view.hpp"
#include "../model/translation.hpp"
#include "../repo/repository.hpp"
#include <shared/paging.hpp>

#include <userver/components/loggable_component_base.hpp>
#include <userver/yaml_config/schema.hpp>
#include <userver/utest/using_namespace_userver.hpp>
#include <userver/http/url.hpp>
#include <userver/utils/boost_uuid4.hpp>
#include <utility>

namespace svetit::project {

class Repository;

class Service final : public components::LoggableComponentBase {
public:
	static constexpr std::string_view kName = "main-service";
	static yaml_config::Schema GetStaticConfigSchema();

	explicit Service(
		const components::ComponentConfig& conf,
		const components::ComponentContext& ctx);

	model::Project GetProjectById(const boost::uuids::uuid& id);
	model::Project GetProjectByKey(const std::string& key);
	void CreateProject(const model::Project& project);
	void UpdateProject(const model::Project& project);
	void DeleteProject(const boost::uuids::uuid& id);
	PagingResult<model::Project> GetProjectList(uint32_t start, uint32_t limit);

	model::ProjectParam GetProjectParam(const boost::uuids::uuid& projectId, int paramId);
	void CreateProjectParam(const model::ProjectParam& projectParam);
	void DeleteProjectParam(const boost::uuids::uuid& projectId, int paramId);
	PagingResult<model::ProjectParam> GetProjectParamList(uint32_t start, uint32_t limit);

	PagingResult<model::Section> GetSectionList(uint32_t start, uint32_t limit);
	PagingResult<model::ParamType> GetParamTypeList(uint32_t start, uint32_t limit);

	model::SectionParam GetSectionParam(int sectionId, int paramId);
	void CreateSectionParam(const model::SectionParam& sectionParam);
	void DeleteSectionParam(int sectionId, int paramId);
	PagingResult<model::SectionParam> GetSectionParamList(uint32_t start, uint32_t limit);

	PagingResult<model::CcType> GetCcTypeList(uint32_t start, uint32_t limit);

	model::ControlCircuit GetControlCircuit(int id);
	void CreateControlCircuit(const model::ControlCircuit& сontrolCircuit);
	void UpdateControlCircuit(const model::ControlCircuit& сontrolCircuit);
	void DeleteControlCircuit(int id);
	PagingResult<model::ControlCircuit> GetControlCircuitList(uint32_t start, uint32_t limit);

	model::Plugin GetPlugin(int id);
	void CreatePlugin(const model::Plugin& plugin);
	void UpdatePlugin(const model::Plugin& plugin);
	void DeletePlugin(int id);
	PagingResult<model::Plugin> GetPluginList(uint32_t start, uint32_t limit);

	model::Device GetDevice(int id);
	void CreateDevice(const model::Device& device);
	void UpdateDevice(const model::Device& device);
	void DeleteDevice(int id);
	PagingResult<model::Device> GetDeviceList(uint32_t start, uint32_t limit);

	model::DevicePluginParam GetDevicePluginParam(int deviceId, int paramId);
	void CreateDevicePluginParam(const model::DevicePluginParam& devicePluginParam);
	void DeleteDevicePluginParam(int deviceId, int paramId);
	PagingResult<model::DevicePluginParam> GetDevicePluginParamList(uint32_t start, uint32_t limit);

	PagingResult<model::Code> GetCodeList(uint32_t start, uint32_t limit);

	model::Measure GetMeasure(int id);
	void CreateMeasure(const model::Measure& measure);
	void UpdateMeasure(const model::Measure& measure);
	void DeleteMeasure(int id);
	PagingResult<model::Measure> GetMeasureList(uint32_t start, uint32_t limit);

	model::SaveTimer GetSaveTimer(int id);
	void CreateSaveTimer(const model::SaveTimer& saveTimer);
	void UpdateSaveTimer(const model::SaveTimer& saveTimer);
	void DeleteSaveTimer(int id);
	PagingResult<model::SaveTimer> GetSaveTimerList(uint32_t start, uint32_t limit);

	model::CcTypeParam GetCcTypeParam(int ccTypeId, int paramId);
	void CreateCcTypeParam(const model::CcTypeParam& ccTypeParam);
	void DeleteCcTypeParam(int ccTypeId, int paramId);
	PagingResult<model::CcTypeParam> GetCcTypeParamList(uint32_t start, uint32_t limit);

	model::DiType GetDiType(int id);
	void CreateDiType(const model::DiType& diType);
	void UpdateDiType(const model::DiType& diType);
	void DeleteDiType(int id);
	PagingResult<model::DiType> GetDiTypeList(uint32_t start, uint32_t limit);

	model::DiPluginParam GetDiPluginParam(int diTypeId, int paramId);
	void CreateDiPluginParam(const model::DiPluginParam& diPluginParam);
	void DeleteDiPluginParam(int diTypeId, int paramId);
	PagingResult<model::DiPluginParam> GetDiPluginParamList(uint32_t start, uint32_t limit);

	model::CcTypeDiType GetCcTypeDiType(int ccTypeId, int diTypeId);
	void CreateCcTypeDiType(const model::CcTypeDiType& ccTypeDiType);
	void DeleteCcTypeDiType(int ccTypeId, int diTypeId);
	PagingResult<model::CcTypeDiType> GetCcTypeDiTypeList(uint32_t start, uint32_t limit);

	model::DeviceItem GetDeviceItem(int id);
	void CreateDeviceItem(const model::DeviceItem& deviceItem);
	void UpdateDeviceItem(const model::DeviceItem& deviceItem);
	void DeleteDeviceItem(int id);
	PagingResult<model::DeviceItem> GetDeviceItemList(uint32_t start, uint32_t limit);

	PagingResult<model::CcModeType> GetCcModeTypeList(uint32_t start, uint32_t limit);

	model::CcDi GetCcDi(int ccId, int diId);
	void CreateCcDi(const model::CcDi& ccDi);
	void DeleteCcDi(int ccId, int diId);
	PagingResult<model::CcDi> GetCcDiList(uint32_t start, uint32_t limit);

	model::CcParam GetCcParam(int ccId, int paramId);
	void CreateCcParam(const model::CcParam& ccParam);
	void DeleteCcParam(int ccId, int paramId);
	PagingResult<model::CcParam> GetCcParamList(uint32_t start, uint32_t limit);

	PagingResult<model::CcStatusCategory> GetCcStatusCategoryList(uint32_t start, uint32_t limit);

	PagingResult<model::CcStatusType> GetCcStatusTypeList(uint32_t start, uint32_t limit);

	model::ValueView GetValueView(int id);
	void CreateValueView(const model::ValueView& valueView);
	void UpdateValueView(const model::ValueView& valueView);
	void DeleteValueView(int id);
	PagingResult<model::ValueView> GetValueViewList(uint32_t start, uint32_t limit);

	model::Translation GetTranslation(int id);
	void CreateTranslation(const model::Translation& translation);
	void UpdateTranslation(const model::Translation& translation);
	void DeleteTranslation(int id);
	PagingResult<model::Translation> GetTranslationList(uint32_t start, uint32_t limit);

	const std::string& GetJSONSchemasPath();

	Repository& Repo();

private:
	Repository& _repo;
	int _itemsLimitForList;
	std::string _jsonSchemasPath;
};

} // namespace svetit::project
