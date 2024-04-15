#include "service.hpp"

#include <boost/uuid/uuid_generators.hpp>

#include "../repo/repository.hpp"
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
)");
}

Service::Service(
		const components::ComponentConfig& conf,
		const components::ComponentContext& ctx)
	: components::LoggableComponentBase{conf, ctx}
	, _repo{ctx.FindComponent<Repository>()}
	, _itemsLimitForList{conf["items-limit-for-list"].As<int>()}
{}

model::Project Service::GetProjectById(const boost::uuids::uuid& id) {
	return _repo.Project().SelectById(id);
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
	return _repo.ProjectParam().Select(projectId, paramId);
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

model::Section Service::GetSection(int id) {
	return _repo.Section().Select(id);
}

void Service::CreateSection(const model::Section& section) {
	_repo.Section().Insert(section.projectId, section.name);
}

void Service::UpdateSection(const model::Section& section) {
	_repo.Section().Update(section);
}

void Service::DeleteSection(int id) {
	_repo.Section().Delete(id);
}

PagingResult<model::Section> Service::GetSectionList(uint32_t start, uint32_t limit) {
	return _repo.Section().GetList(start, limit);
}

model::ParamType Service::GetParamType(int id) {
	return _repo.ParamType().Select(id);
}

void Service::CreateParamType(const model::ParamType& paramType) {
	_repo.ParamType().Insert(paramType.parentId, paramType.key, paramType.name, paramType.description, paramType.valueType);
}

void Service::UpdateParamType(const model::ParamType& paramType) {
	_repo.ParamType().Update(paramType);
}

void Service::DeleteParamType(int id) {
	_repo.ParamType().Delete(id);
}

PagingResult<model::ParamType> Service::GetParamTypeList(uint32_t start, uint32_t limit) {
	return _repo.ParamType().GetList(start, limit);
}

model::SectionParam Service::GetSectionParam(int sectionId, int paramId) {
	return _repo.SectionParam().Select(sectionId, paramId);
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
	return _repo.CcType().Select(id);
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
	return _repo.ControlCircuit().Select(id);
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
	return _repo.Plugin().Select(id);
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
	return _repo.Device().Select(id);
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
	return _repo.DevicePluginParam().Select(deviceId, paramId);
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
	return _repo.Code().Select(id);
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
	return _repo.Measure().Select(id);
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
	return _repo.SaveTimer().Select(id);
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
	return _repo.CcTypeParam().Select(ccTypeId, paramId);
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
	return _repo.DiType().Select(id);
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

} // namespace svetit::project
