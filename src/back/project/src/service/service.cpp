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

PagingResult<model::Project> Service::GetProjectList(const boost::uuids::uuid& spaceId, uint32_t start, uint32_t limit) {
	return _repo.Project().GetList(start, limit);
}

model::ProjectParam Service::GetProjectParam(const boost::uuids::uuid& spaceId, const boost::uuids::uuid& projectId, int paramId) {
	return _repo.ProjectParam().Get(spaceId, projectId, paramId);
}

void Service::CreateProjectParam(const model::ProjectParam& projectParam) {
	_repo.ProjectParam().Insert(projectParam.projectId, projectParam.paramId);
}

void Service::DeleteProjectParam(const boost::uuids::uuid& spaceId, const boost::uuids::uuid& projectId, int paramId) {
	_repo.ProjectParam().Delete(spaceId, projectId, paramId);
}

PagingResult<model::ProjectParam> Service::GetProjectParamList(const boost::uuids::uuid& spaceId, uint32_t start, uint32_t limit) {
	return _repo.ProjectParam().GetList(spaceId, start, limit);
}

PagingResult<model::Section> Service::GetSectionList(const boost::uuids::uuid& spaceId, uint32_t start, uint32_t limit) {
	return _repo.Section().GetList(spaceId, start, limit);
}

PagingResult<model::ParamType> Service::GetParamTypeList(const boost::uuids::uuid& spaceId, uint32_t start, uint32_t limit) {
	return _repo.ParamType().GetList(spaceId, start, limit);
}

model::SectionParam Service::GetSectionParam(const boost::uuids::uuid& spaceId, int sectionId, int paramId) {
	return _repo.SectionParam().Get(spaceId, sectionId, paramId);
}

void Service::CreateSectionParam(const model::SectionParam& sectionParam) {
	_repo.SectionParam().Insert(sectionParam.sectionId, sectionParam.paramId);
}

void Service::DeleteSectionParam(const boost::uuids::uuid& spaceId, int sectionId, int paramId) {
	_repo.SectionParam().Delete(spaceId, sectionId, paramId);
}

PagingResult<model::SectionParam> Service::GetSectionParamList(const boost::uuids::uuid& spaceId, uint32_t start, uint32_t limit) {
	return _repo.SectionParam().GetList(spaceId, start, limit);
}

PagingResult<model::CcType> Service::GetCcTypeList(const boost::uuids::uuid& spaceId, uint32_t start, uint32_t limit) {
	return _repo.CcType().GetList(spaceId, start, limit);
}

PagingResult<model::ControlCircuit> Service::GetControlCircuitList(const boost::uuids::uuid& spaceId, uint32_t start, uint32_t limit) {
	return _repo.ControlCircuit().GetList(spaceId, start, limit);
}

PagingResult<model::Plugin> Service::GetPluginList(const boost::uuids::uuid& spaceId, uint32_t start, uint32_t limit) {
	return _repo.Plugin().GetList(spaceId, start, limit);
}

PagingResult<model::Device> Service::GetDeviceList(const boost::uuids::uuid& spaceId, uint32_t start, uint32_t limit) {
	return _repo.Device().GetList(spaceId, start, limit);
}

model::DevicePluginParam Service::GetDevicePluginParam(const boost::uuids::uuid& spaceId, int deviceId, int paramId) {
	return _repo.DevicePluginParam().Get(spaceId, deviceId, paramId);
}

void Service::CreateDevicePluginParam(const model::DevicePluginParam& devicePluginParam) {
	_repo.DevicePluginParam().Insert(devicePluginParam.deviceId, devicePluginParam.paramId);
}

void Service::DeleteDevicePluginParam(const boost::uuids::uuid& spaceId, int deviceId, int paramId) {
	_repo.DevicePluginParam().Delete(spaceId, deviceId, paramId);
}

PagingResult<model::DevicePluginParam> Service::GetDevicePluginParamList(const boost::uuids::uuid& spaceId, uint32_t start, uint32_t limit) {
	return _repo.DevicePluginParam().GetList(spaceId, start, limit);
}

PagingResult<model::Code> Service::GetCodeList(const boost::uuids::uuid& spaceId, uint32_t start, uint32_t limit) {
	return _repo.Code().GetList(spaceId, start, limit);
}

PagingResult<model::Measure> Service::GetMeasureList(const boost::uuids::uuid& spaceId, uint32_t start, uint32_t limit) {
	return _repo.Measure().GetList(spaceId, start, limit);
}

PagingResult<model::SaveTimer> Service::GetSaveTimerList(const boost::uuids::uuid& spaceId, uint32_t start, uint32_t limit) {
	return _repo.SaveTimer().GetList(spaceId, start, limit);
}

model::CcTypeParam Service::GetCcTypeParam(const boost::uuids::uuid& spaceId, int ccTypeId, int paramId) {
	return _repo.CcTypeParam().Get(spaceId, ccTypeId, paramId);
}

void Service::CreateCcTypeParam(const model::CcTypeParam& ccTypeParam) {
	_repo.CcTypeParam().Insert(ccTypeParam.ccTypeId, ccTypeParam.paramId);
}

void Service::DeleteCcTypeParam(const boost::uuids::uuid& spaceId, int ccTypeId, int paramId) {
	_repo.CcTypeParam().Delete(spaceId, ccTypeId, paramId);
}

PagingResult<model::CcTypeParam> Service::GetCcTypeParamList(const boost::uuids::uuid& spaceId, uint32_t start, uint32_t limit) {
	return _repo.CcTypeParam().GetList(spaceId, start, limit);
}

PagingResult<model::DiType> Service::GetDiTypeList(const boost::uuids::uuid& spaceId, uint32_t start, uint32_t limit) {
	return _repo.DiType().GetList(spaceId, start, limit);
}

model::DiPluginParam Service::GetDiPluginParam(const boost::uuids::uuid& spaceId, int diTypeId, int paramId) {
	return _repo.DiPluginParam().Get(spaceId, diTypeId, paramId);
}

void Service::CreateDiPluginParam(const model::DiPluginParam& diPluginParam) {
	_repo.DiPluginParam().Insert(diPluginParam.diTypeId, diPluginParam.paramId);
}

void Service::DeleteDiPluginParam(const boost::uuids::uuid& spaceId, int diTypeId, int paramId) {
	_repo.DiPluginParam().Delete(spaceId, diTypeId, paramId);
}

PagingResult<model::DiPluginParam> Service::GetDiPluginParamList(const boost::uuids::uuid& spaceId, uint32_t start, uint32_t limit) {
	return _repo.DiPluginParam().GetList(spaceId, start, limit);
}

model::CcTypeDiType Service::GetCcTypeDiType(const boost::uuids::uuid& spaceId, int ccTypeId, int diTypeId) {
	return _repo.CcTypeDiType().Get(spaceId, ccTypeId, diTypeId);
}

void Service::CreateCcTypeDiType(const model::CcTypeDiType& ccTypeDiType) {
	_repo.CcTypeDiType().Insert(ccTypeDiType.ccTypeId, ccTypeDiType.diTypeId);
}

void Service::DeleteCcTypeDiType(const boost::uuids::uuid& spaceId, int ccTypeId, int diTypeId) {
	_repo.CcTypeDiType().Delete(spaceId, ccTypeId, diTypeId);
}

PagingResult<model::CcTypeDiType> Service::GetCcTypeDiTypeList(const boost::uuids::uuid& spaceId, uint32_t start, uint32_t limit) {
	return _repo.CcTypeDiType().GetList(spaceId, start, limit);
}

model::CcDi Service::GetCcDi(const boost::uuids::uuid& spaceId, int ccId, int diId) {
	return _repo.CcDi().Get(spaceId, ccId, diId);
}

void Service::CreateCcDi(const boost::uuids::uuid& spaceId, const model::CcDi& ccDi) {
	_repo.CcDi().Insert(ccDi.ccId, ccDi.diId);
}

void Service::DeleteCcDi(const boost::uuids::uuid& spaceId, int ccId, int diId) {
	_repo.CcDi().Delete(spaceId, ccId, diId);
}

PagingResult<model::CcDi> Service::GetCcDiList(const boost::uuids::uuid& spaceId, uint32_t start, uint32_t limit) {
	return _repo.CcDi().GetList(spaceId, start, limit);
}

model::CcParam Service::GetCcParam(const boost::uuids::uuid& spaceId, int ccId, int paramId) {
	return _repo.CcParam().Get(spaceId, ccId, paramId);
}

void Service::CreateCcParam(const model::CcParam& ccParam) {
	_repo.CcParam().Insert(ccParam.ccId, ccParam.paramId);
}

void Service::DeleteCcParam(const boost::uuids::uuid& spaceId, int ccId, int paramId) {
	_repo.CcParam().Delete(spaceId, ccId, paramId);
}

PagingResult<model::CcParam> Service::GetCcParamList(const boost::uuids::uuid& spaceId, uint32_t start, uint32_t limit) {
	return _repo.CcParam().GetList(spaceId, start, limit);
}

PagingResult<model::CcStatusCategory> Service::GetCcStatusCategoryList(const boost::uuids::uuid& spaceId, uint32_t start, uint32_t limit) {
	return _repo.CcStatusCategory().GetList(spaceId, start, limit);
}

PagingResult<model::CcStatusType> Service::GetCcStatusTypeList(const boost::uuids::uuid& spaceId, uint32_t start, uint32_t limit) {
	return _repo.CcStatusType().GetList(spaceId, start, limit);
}

PagingResult<model::ValueView> Service::GetValueViewList(const boost::uuids::uuid& spaceId, uint32_t start, uint32_t limit) {
	return _repo.ValueView().GetList(spaceId, start, limit);
}

PagingResult<model::Translation> Service::GetTranslationList(const boost::uuids::uuid& spaceId, uint32_t start, uint32_t limit) {
	return _repo.Translation().GetList(spaceId, start, limit);
}

const std::string& Service::GetJSONSchemasPath() {
	return _jsonSchemasPath;
}

Repository& Service::Repo() {
	return _repo;
}

} // namespace svetit::project
