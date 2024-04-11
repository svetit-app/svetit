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
	const auto res = _repo.Project().Insert(project.spaceId, project.key, project.name, project.description, project.changedAt, project.sync);
	if (res.is_nil())
		throw errors::BadRequest400{"Project not created"};
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
	_repo.ProjectParam().Insert(projectParam.projectId, projectParam.paramId, projectParam.isDeleted);
	// is needed to check returned value from insert?
}

void Service::UpdateProjectParam(const model::ProjectParam& projectParam) {
	_repo.ProjectParam().Update(projectParam);
}

void Service::DeleteProjectParam(const boost::uuids::uuid& projectId, int paramId) {
	_repo.ProjectParam().Delete(projectId, paramId);
}

PagingResult<model::ProjectParam> Service::GetProjectParamList(uint32_t start, uint32_t limit, bool keepDeleted) {
	if (keepDeleted)
		return _repo.ProjectParam().GetList(start, limit);
	else
		return _repo.ProjectParam().GetListNoDeleted(start, limit);
}

model::Section Service::GetSection(int id) {
	return _repo.Section().Select(id);
}

void Service::CreateSection(const model::Section& section) {
	_repo.Section().Insert(section.projectId, section.name, section.isDeleted);
	// is needed to check returned value from insert?
}

void Service::UpdateSection(const model::Section& section) {
	_repo.Section().Update(section);
}

void Service::DeleteSection(int id) {
	_repo.Section().Delete(id);
}

PagingResult<model::Section> Service::GetSectionList(uint32_t start, uint32_t limit, bool keepDeleted) {
	if (keepDeleted)
		return _repo.Section().GetList(start, limit);
	else
		return _repo.Section().GetListNoDeleted(start, limit);
}

} // namespace svetit::project
