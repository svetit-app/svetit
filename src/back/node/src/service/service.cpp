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

model::Node Service::Get(const boost::uuids::uuid& id, const std::string& userId, const boost::uuids::uuid& spaceId) {
	return _repo.Node().Select(id, spaceId);
}

void Service::Delete(const boost::uuids::uuid& id, const std::string& userId, const boost::uuids::uuid& spaceId) {
	_repo.Node().Delete(id, spaceId);
}

void Service::Create(const model::Node& item, const std::string& userId, const boost::uuids::uuid& spaceId) {
	_repo.Node().Create(item, spaceId);
}

void Service::Update(const model::Node& item, const std::string& userId, const boost::uuids::uuid& spaceId) {
	_repo.Node().Update(item, spaceId);
}

PagingResult<model::Node> Service::GetList(const std::string& userId, const boost::uuids::uuid& spaceId, const std::string& role, uint32_t start, uint32_t limit) {
	if (role == "admin")
		return _repo.Node().SelectList(spaceId, start, limit);
	return _repo.SelectNodeList(spaceId, userId, start, limit);
}

model::NodeProject Service::GetNodeProject(const boost::uuids::uuid& nodeId, const boost::uuids::uuid& projectId, const std::string& userId) {
	return _repo.NodeProject().Select(nodeId, projectId);
}

void Service::DeleteNodeProject(const boost::uuids::uuid& nodeId, const boost::uuids::uuid& projectId, const std::string& userId) {
	_repo.NodeProject().Delete(nodeId, projectId);
}

void Service::CreateNodeProject(const model::NodeProject& item, const std::string& userId) {
	_repo.NodeProject().Create(item);
}

PagingResult<model::NodeProject> Service::GetNodeProjectListByNodeId(const std::string& userId, uint32_t start, uint32_t limit, const boost::uuids::uuid& nodeId) {
	return _repo.NodeProject().SelectListByNodeId(start, limit, nodeId);
}

PagingResult<model::NodeProject> Service::GetNodeProjectList(const std::string& userId, uint32_t start, uint32_t limit) {
	return _repo.NodeProject().SelectList(start, limit);
}

model::NodeGroup Service::GetNodeGroup(const boost::uuids::uuid& nodeId, int groupId, const std::string& userId) {
	return _repo.NodeGroup().Select(nodeId, groupId);
}
void Service::DeleteNodeGroup(const boost::uuids::uuid& nodeId, int groupId, const std::string& userId) {
	_repo.NodeGroup().Delete(nodeId, groupId);
}

void Service::CreateNodeGroup(const model::NodeGroup& item, const std::string& userId) {
	_repo.NodeGroup().Create(item);
}

PagingResult<model::NodeGroup> Service::GetNodeGroupList(const std::string& userId, uint32_t start, uint32_t limit) {
	return _repo.NodeGroup().SelectList(start, limit);
}

PagingResult<model::NodeGroup> Service::GetNodeGroupListByNodeId(const std::string& userId, uint32_t start, uint32_t limit, const boost::uuids::uuid& nodeId) {
	return _repo.NodeGroup().SelectListByNodeId(start, limit, nodeId);
}

model::GroupUser Service::GetGroupUser(int groupId, const std::string& userId, const std::string& headerUserId) {
	return _repo.GroupUser().Select(groupId, userId);
}

void Service::DeleteGroupUser(int groupId, const std::string& userId, const std::string& headerUserId) {
	_repo.GroupUser().Delete(groupId, userId);
}

void Service::CreateGroupUser(const model::GroupUser& item, const std::string& headerUserId) {
	_repo.GroupUser().Create(item);
}

PagingResult<model::GroupUser> Service::GetGroupUserList(const std::string& headerUserId, uint32_t start, uint32_t limit) {
	return _repo.GroupUser().SelectList(start, limit);
}

PagingResult<model::GroupUser> Service::GetGroupUserListByGroupId(const std::string& headerUserId, uint32_t start, uint32_t limit, int groupId) {
	return _repo.GroupUser().SelectListByGroupId(start, limit, groupId);
}

} // namespace svetit::node
