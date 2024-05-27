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

const std::string& Service::GetJSONSchemasPath() {
	return _jsonSchemasPath;
}

Repository& Service::Repo() {
	return _repo;
}

} // namespace svetit::project
