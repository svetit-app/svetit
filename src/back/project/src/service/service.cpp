#include "service.hpp"

#include <boost/uuid/uuid_generators.hpp>

#include <shared/errors.hpp>
#include <shared/paging.hpp>

#include <userver/yaml_config/merge_schemas.hpp>
#include <userver/components/component_config.hpp>
#include <userver/components/component_context.hpp>
#include <userver/formats/serialize/common_containers.hpp>

namespace svetit::project {

/*static*/ yaml_config::Schema Service::GetStaticConfigSchema()
{
	return yaml_config::MergeSchemas<components::LoggableComponentBase>(R"(
type: object
description: Main service component
additionalProperties: false
properties:
  items-limit-for-list:
    type: string
    description: How many items list may contain
  json-schemas-path:
    type: string
    description: Path to folder with JSON schemas
)");
}

Service::Service(
		const components::ComponentConfig& conf,
		const components::ComponentContext& ctx)
	: components::LoggableComponentBase{conf, ctx}
	, _repo{ctx.FindComponent<Repository>()}
	, _itemsLimitForList{conf["items-limit-for-list"].As<int>()}
	, _jsonSchemasPath{conf["json-schemas-path"].As<std::string>()}
{}

model::Project Service::GetProjectById(const boost::uuids::uuid& id) {
	return _repo.Project().Get(id);
}

model::Project Service::GetProjectByKey(const std::string& key) {
	return _repo.Project().SelectByKey(key);
}

void Service::CreateProject(const model::Project& project) {
	_repo.Project().Insert(project.spaceId, project.key, project.name, project.description, project.changedAt, project.sync);
}

void Service::UpdateProject(const model::Project& project) {
	_repo.Project().Update(project);
}

void Service::DeleteProject(const boost::uuids::uuid& id) {
	_repo.Project().Delete(id);
}

PagingResult<model::Project> Service::GetProjectList(uint32_t start, uint32_t limit) {
	return _repo.Project().GetList(start, limit);
}

model::ProjectParam Service::GetProjectParam(const boost::uuids::uuid& projectId, int paramId) {
	return _repo.ProjectParam().Get(projectId, paramId);
}

void Service::CreateProjectParam(const model::ProjectParam& projectParam) {
	_repo.ProjectParam().Insert(projectParam.projectId, projectParam.paramId);
}

void Service::DeleteProjectParam(const boost::uuids::uuid& projectId, int paramId) {
	_repo.ProjectParam().Delete(projectId, paramId);
}

PagingResult<model::ProjectParam> Service::GetProjectParamList(uint32_t start, uint32_t limit) {
	return _repo.ProjectParam().GetList(start, limit);
}

PagingResult<model::Section> Service::GetSectionList(uint32_t start, uint32_t limit) {
	return _repo.Section().GetList(start, limit);
}

PagingResult<model::ParamType> Service::GetParamTypeList(uint32_t start, uint32_t limit) {
	return _repo.ParamType().GetList(start, limit);
}

model::SectionParam Service::GetSectionParam(int sectionId, int paramId) {
	return _repo.SectionParam().Get(sectionId, paramId);
}

void Service::CreateSectionParam(const model::SectionParam& sectionParam) {
	_repo.SectionParam().Insert(sectionParam.sectionId, sectionParam.paramId);
}

void Service::DeleteSectionParam(int sectionId, int paramId) {
	_repo.SectionParam().Delete(sectionId, paramId);
}

PagingResult<model::SectionParam> Service::GetSectionParamList(uint32_t start, uint32_t limit) {
	return _repo.SectionParam().GetList(start, limit);
}

model::CcType Service::GetCcType(int id) {
	return _repo.CcType().Get(id);
}

void Service::CreateCcType(const model::CcType& ccType) {
	_repo.CcType().Insert(ccType.projectId, ccType.key, ccType.name, ccType.description);
}

void Service::UpdateCcType(const model::CcType& ccType) {
	_repo.CcType().Update(ccType);
}

void Service::DeleteCcType(int id) {
	_repo.CcType().Delete(id);
}

PagingResult<model::CcType> Service::GetCcTypeList(uint32_t start, uint32_t limit) {
	return _repo.CcType().GetList(start, limit);
}

model::ControlCircuit Service::GetControlCircuit(int id) {
	return _repo.ControlCircuit().Get(id);
}

void Service::CreateControlCircuit(const model::ControlCircuit& controlCircuit) {
	_repo.ControlCircuit().Insert(controlCircuit.typeId, controlCircuit.sectionId, controlCircuit.name);
}

void Service::UpdateControlCircuit(const model::ControlCircuit& controlCircuit) {
	_repo.ControlCircuit().Update(controlCircuit);
}

void Service::DeleteControlCircuit(int id) {
	_repo.ControlCircuit().Delete(id);
}

PagingResult<model::ControlCircuit> Service::GetControlCircuitList(uint32_t start, uint32_t limit) {
	return _repo.ControlCircuit().GetList(start, limit);
}

model::Plugin Service::GetPlugin(int id) {
	return _repo.Plugin().Get(id);
}

void Service::CreatePlugin(const model::Plugin& plugin) {
	_repo.Plugin().Insert(plugin.projectId, plugin.name, plugin.description, plugin.key);
}

void Service::UpdatePlugin(const model::Plugin& plugin) {
	_repo.Plugin().Update(plugin);
}

void Service::DeletePlugin(int id) {
	_repo.Plugin().Delete(id);
}

PagingResult<model::Plugin> Service::GetPluginList(uint32_t start, uint32_t limit) {
	return _repo.Plugin().GetList(start, limit);
}

model::Device Service::GetDevice(int id) {
	return _repo.Device().Get(id);
}

void Service::CreateDevice(const model::Device& device) {
	_repo.Device().Insert(device.projectId, device.pluginId, device.name, device.checkIntervalMsec);
}

void Service::UpdateDevice(const model::Device& device) {
	_repo.Device().Update(device);
}

void Service::DeleteDevice(int id) {
	_repo.Device().Delete(id);
}

PagingResult<model::Device> Service::GetDeviceList(uint32_t start, uint32_t limit) {
	return _repo.Device().GetList(start, limit);
}

model::DevicePluginParam Service::GetDevicePluginParam(int deviceId, int paramId) {
	return _repo.DevicePluginParam().Get(deviceId, paramId);
}

void Service::CreateDevicePluginParam(const model::DevicePluginParam& devicePluginParam) {
	_repo.DevicePluginParam().Insert(devicePluginParam.deviceId, devicePluginParam.paramId);
}

void Service::DeleteDevicePluginParam(int deviceId, int paramId) {
	_repo.DevicePluginParam().Delete(deviceId, paramId);
}

PagingResult<model::DevicePluginParam> Service::GetDevicePluginParamList(uint32_t start, uint32_t limit) {
	return _repo.DevicePluginParam().GetList(start, limit);
}

model::Code Service::GetCode(int id) {
	return _repo.Code().Get(id);
}

void Service::CreateCode(const model::Code& code) {
	_repo.Code().Insert(code.projectId, code.repositoryId, code.commitHash);
}

void Service::UpdateCode(const model::Code& code) {
	_repo.Code().Update(code);
}

void Service::DeleteCode(int id) {
	_repo.Code().Delete(id);
}

PagingResult<model::Code> Service::GetCodeList(uint32_t start, uint32_t limit) {
	return _repo.Code().GetList(start, limit);
}

model::Measure Service::GetMeasure(int id) {
	return _repo.Measure().Get(id);
}

void Service::CreateMeasure(const model::Measure& measure) {
	_repo.Measure().Insert(measure.projectId, measure.name);
}

void Service::UpdateMeasure(const model::Measure& measure) {
	_repo.Measure().Update(measure);
}

void Service::DeleteMeasure(int id) {
	_repo.Measure().Delete(id);
}

PagingResult<model::Measure> Service::GetMeasureList(uint32_t start, uint32_t limit) {
	return _repo.Measure().GetList(start, limit);
}

model::SaveTimer Service::GetSaveTimer(int id) {
	return _repo.SaveTimer().Get(id);
}

void Service::CreateSaveTimer(const model::SaveTimer& saveTimer) {
	_repo.SaveTimer().Insert(saveTimer.projectId, saveTimer.intervalMsec);
}

void Service::UpdateSaveTimer(const model::SaveTimer& saveTimer) {
	_repo.SaveTimer().Update(saveTimer);
}

void Service::DeleteSaveTimer(int id) {
	_repo.SaveTimer().Delete(id);
}

PagingResult<model::SaveTimer> Service::GetSaveTimerList(uint32_t start, uint32_t limit) {
	return _repo.SaveTimer().GetList(start, limit);
}

model::CcTypeParam Service::GetCcTypeParam(int ccTypeId, int paramId) {
	return _repo.CcTypeParam().Get(ccTypeId, paramId);
}

void Service::CreateCcTypeParam(const model::CcTypeParam& ccTypeParam) {
	_repo.CcTypeParam().Insert(ccTypeParam.ccTypeId, ccTypeParam.paramId);
}

void Service::DeleteCcTypeParam(int ccTypeId, int paramId) {
	_repo.CcTypeParam().Delete(ccTypeId, paramId);
}

PagingResult<model::CcTypeParam> Service::GetCcTypeParamList(uint32_t start, uint32_t limit) {
	return _repo.CcTypeParam().GetList(start, limit);
}

model::DiType Service::GetDiType(int id) {
	return _repo.DiType().Get(id);
}

void Service::CreateDiType(const model::DiType& diType) {
	_repo.DiType().Insert(diType.measureId, diType.saveTimerId, diType.key, diType.name, diType.mode, diType.saveAlgorithm);
}

void Service::UpdateDiType(const model::DiType& diType) {
	_repo.DiType().Update(diType);
}

void Service::DeleteDiType(int id) {
	_repo.DiType().Delete(id);
}

PagingResult<model::DiType> Service::GetDiTypeList(uint32_t start, uint32_t limit) {
	return _repo.DiType().GetList(start, limit);
}

model::DiPluginParam Service::GetDiPluginParam(int diTypeId, int paramId) {
	return _repo.DiPluginParam().Get(diTypeId, paramId);
}

void Service::CreateDiPluginParam(const model::DiPluginParam& diPluginParam) {
	_repo.DiPluginParam().Insert(diPluginParam.diTypeId, diPluginParam.paramId);
}

void Service::DeleteDiPluginParam(int diTypeId, int paramId) {
	_repo.DiPluginParam().Delete(diTypeId, paramId);
}

PagingResult<model::DiPluginParam> Service::GetDiPluginParamList(uint32_t start, uint32_t limit) {
	return _repo.DiPluginParam().GetList(start, limit);
}

model::CcTypeDiType Service::GetCcTypeDiType(int ccTypeId, int diTypeId) {
	return _repo.CcTypeDiType().Get(ccTypeId, diTypeId);
}

void Service::CreateCcTypeDiType(const model::CcTypeDiType& ccTypeDiType) {
	_repo.CcTypeDiType().Insert(ccTypeDiType.ccTypeId, ccTypeDiType.diTypeId);
}

void Service::DeleteCcTypeDiType(int ccTypeId, int diTypeId) {
	_repo.CcTypeDiType().Delete(ccTypeId, diTypeId);
}

PagingResult<model::CcTypeDiType> Service::GetCcTypeDiTypeList(uint32_t start, uint32_t limit) {
	return _repo.CcTypeDiType().GetList(start, limit);
}

model::DeviceItem Service::GetDeviceItem(int id) {
	return _repo.DeviceItem().Get(id);
}

void Service::CreateDeviceItem(const model::DeviceItem& deviceItem) {
	_repo.DeviceItem().Insert(deviceItem.deviceId, deviceItem.typeId, deviceItem.name);
}

void Service::UpdateDeviceItem(const model::DeviceItem& deviceItem) {
	_repo.DeviceItem().Update(deviceItem);
}

void Service::DeleteDeviceItem(int id) {
	_repo.DeviceItem().Delete(id);
}

PagingResult<model::DeviceItem> Service::GetDeviceItemList(uint32_t start, uint32_t limit) {
	return _repo.DeviceItem().GetList(start, limit);
}

model::CcModeType Service::GetCcModeType(int id) {
	return _repo.CcModeType().Get(id);
}

void Service::CreateCcModeType(const model::CcModeType& ccModeType) {
	_repo.CcModeType().Insert(ccModeType.ccTypeId, ccModeType.key, ccModeType.name);
}

void Service::UpdateCcModeType(const model::CcModeType& ccModeType) {
	_repo.CcModeType().Update(ccModeType);
}

void Service::DeleteCcModeType(int id) {
	_repo.CcModeType().Delete(id);
}

PagingResult<model::CcModeType> Service::GetCcModeTypeList(uint32_t start, uint32_t limit) {
	return _repo.CcModeType().GetList(start, limit);
}

model::CcDi Service::GetCcDi(int ccId, int diId) {
	return _repo.CcDi().Get(ccId, diId);
}

void Service::CreateCcDi(const model::CcDi& ccDi) {
	_repo.CcDi().Insert(ccDi.ccId, ccDi.diId);
}

void Service::DeleteCcDi(int ccId, int diId) {
	_repo.CcDi().Delete(ccId, diId);
}

PagingResult<model::CcDi> Service::GetCcDiList(uint32_t start, uint32_t limit) {
	return _repo.CcDi().GetList(start, limit);
}

model::CcParam Service::GetCcParam(int ccId, int paramId) {
	return _repo.CcParam().Get(ccId, paramId);
}

void Service::CreateCcParam(const model::CcParam& ccParam) {
	_repo.CcParam().Insert(ccParam.ccId, ccParam.paramId);
}

void Service::DeleteCcParam(int ccId, int paramId) {
	_repo.CcParam().Delete(ccId, paramId);
}

PagingResult<model::CcParam> Service::GetCcParamList(uint32_t start, uint32_t limit) {
	return _repo.CcParam().GetList(start, limit);
}

model::CcStatusCategory Service::GetCcStatusCategory(int id) {
	return _repo.CcStatusCategory().Get(id);
}

void Service::CreateCcStatusCategory(const model::CcStatusCategory& ccStatusCategory) {
	_repo.CcStatusCategory().Insert(ccStatusCategory.projectId, ccStatusCategory.key, ccStatusCategory.name, ccStatusCategory.color);
}

void Service::UpdateCcStatusCategory(const model::CcStatusCategory& ccStatusCategory) {
	_repo.CcStatusCategory().Update(ccStatusCategory);
}

void Service::DeleteCcStatusCategory(int id) {
	_repo.CcStatusCategory().Delete(id);
}

PagingResult<model::CcStatusCategory> Service::GetCcStatusCategoryList(uint32_t start, uint32_t limit) {
	return _repo.CcStatusCategory().GetList(start, limit);
}

model::CcStatusType Service::GetCcStatusType(int id) {
	return _repo.CcStatusType().Get(id);
}

void Service::CreateCcStatusType(const model::CcStatusType& ccStatusType) {
	_repo.CcStatusType().Insert(ccStatusType.ccTypeId, ccStatusType.categoryId, ccStatusType.key, ccStatusType.text, ccStatusType.inform);
}

void Service::UpdateCcStatusType(const model::CcStatusType& ccStatusType) {
	_repo.CcStatusType().Update(ccStatusType);
}

void Service::DeleteCcStatusType(int id) {
	_repo.CcStatusType().Delete(id);
}

PagingResult<model::CcStatusType> Service::GetCcStatusTypeList(uint32_t start, uint32_t limit) {
	return _repo.CcStatusType().GetList(start, limit);
}

model::ValueView Service::GetValueView(int id) {
	return _repo.ValueView().Get(id);
}

void Service::CreateValueView(const model::ValueView& valueView) {
	_repo.ValueView().Insert(valueView.diTypeId, valueView.value, valueView.view);
}

void Service::UpdateValueView(const model::ValueView& valueView) {
	_repo.ValueView().Update(valueView);
}

void Service::DeleteValueView(int id) {
	_repo.ValueView().Delete(id);
}

PagingResult<model::ValueView> Service::GetValueViewList(uint32_t start, uint32_t limit) {
	return _repo.ValueView().GetList(start, limit);
}

model::Translation Service::GetTranslation(int id) {
	return _repo.Translation().Get(id);
}

void Service::CreateTranslation(const model::Translation& translation) {
	_repo.Translation().Insert(translation.projectId, translation.lang, translation.key, translation.value);
}

void Service::UpdateTranslation(const model::Translation& translation) {
	_repo.Translation().Update(translation);
}

void Service::DeleteTranslation(int id) {
	_repo.Translation().Delete(id);
}

PagingResult<model::Translation> Service::GetTranslationList(uint32_t start, uint32_t limit) {
	return _repo.Translation().GetList(start, limit);
}

std::string Service::GetJSONSchemasPath() {
	return _jsonSchemasPath;
}

Repository& Service::Repo() {
	return _repo;
}

} // namespace svetit::project
