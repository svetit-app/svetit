#include "service.hpp"

#include <boost/uuid/uuid_generators.hpp>

#include <shared/errors.hpp>
#include <shared/paging.hpp>

#include <userver/yaml_config/merge_schemas.hpp>
#include <userver/components/component_config.hpp>
#include <userver/components/component_context.hpp>
#include <userver/formats/serialize/common_containers.hpp>

namespace svetit::node {

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

const std::string& Service::GetJSONSchemasPath() {
	return _jsonSchemasPath;
}

Repository& Service::Repo() {
	return _repo;
}

model::Node Service::Get(const boost::uuids::uuid& id, const std::string& userId) {
	const auto node = _repo.Node().Select(id);
	return node;
}

void Service::Delete(const boost::uuids::uuid& id, const std::string& userId) {
	_repo.Node().Delete(id);
}

void Service::Create(const model::Node& item, const std::string& userId) {
	_repo.Node().Create(item);
}

void Service::Update(const model::Node& item, const std::string& userId) {
	_repo.Node().Update(item);
}

PagingResult<model::Node> Service::GetList(const std::string& userId, uint32_t start, uint32_t limit) {
	return _repo.Node().SelectList(start, limit);
}

model::NodeProject Service::GetNodeProject(const boost::uuids::uuid& nodeId, const boost::uuids::uuid& projectId, const std::string& userId) {
	return {};
}

void Service::DeleteNodeProject(const boost::uuids::uuid& id, const boost::uuids::uuid& projectId, const std::string& userId) {
	return;
}

void Service::CreateNodeProject(const model::NodeProject& item, const std::string& userId) {
	return;
}

PagingResult<model::NodeProject> Service::GetNodeProjectList(const std::string& userId, uint32_t start, uint32_t limit, const boost::uuids::uuid& nodeId) {
	return {};
}

} // namespace svetit::node
