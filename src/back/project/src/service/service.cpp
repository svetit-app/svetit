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

PagingResult<model::CcType> Service::GetCcTypeList(uint32_t start, uint32_t limit) {
	return _repo.CcType().GetList(start, limit);
}

PagingResult<model::ControlCircuit> Service::GetControlCircuitList(uint32_t start, uint32_t limit) {
	return _repo.ControlCircuit().GetList(start, limit);
}

PagingResult<model::Plugin> Service::GetPluginList(uint32_t start, uint32_t limit) {
	return _repo.Plugin().GetList(start, limit);
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

PagingResult<model::Code> Service::GetCodeList(uint32_t start, uint32_t limit) {
	return _repo.Code().GetList(start, limit);
}

PagingResult<model::Measure> Service::GetMeasureList(uint32_t start, uint32_t limit) {
	return _repo.Measure().GetList(start, limit);
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

PagingResult<model::DeviceItem> Service::GetDeviceItemList(uint32_t start, uint32_t limit) {
	return _repo.DeviceItem().GetList(start, limit);
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

PagingResult<model::CcStatusCategory> Service::GetCcStatusCategoryList(uint32_t start, uint32_t limit) {
	return _repo.CcStatusCategory().GetList(start, limit);
}

PagingResult<model::CcStatusType> Service::GetCcStatusTypeList(uint32_t start, uint32_t limit) {
	return _repo.CcStatusType().GetList(start, limit);
}

PagingResult<model::ValueView> Service::GetValueViewList(uint32_t start, uint32_t limit) {
	return _repo.ValueView().GetList(start, limit);
}

PagingResult<model::Translation> Service::GetTranslationList(uint32_t start, uint32_t limit) {
	return _repo.Translation().GetList(start, limit);
}

const std::string& Service::GetJSONSchemasPath() {
	return _jsonSchemasPath;
}

Repository& Service::Repo() {
	return _repo;
}

} // namespace svetit::project
