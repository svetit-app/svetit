#pragma once

#include "../repo/repository.hpp"
#include "../model/node.hpp"
#include "../model/node_project.hpp"
#include "../model/node_group.hpp"
#include "../model/group_user.hpp"

#include <svetit/paging.hpp>

#include <userver/components/loggable_component_base.hpp>
#include <userver/yaml_config/schema.hpp>
#include <userver/utest/using_namespace_userver.hpp>

namespace svetit::node {

class Repository;

class Service final : public components::LoggableComponentBase {
public:
	static constexpr std::string_view kName = "main-service";
	static yaml_config::Schema GetStaticConfigSchema();

	explicit Service(
		const components::ComponentConfig& conf,
		const components::ComponentContext& ctx);

	const std::string& GetJSONSchemasPath();

	Repository& Repo();

	model::NodeProject GetNodeProject(const boost::uuids::uuid& nodeId, const boost::uuids::uuid& projectId, const std::string& userId);
	void DeleteNodeProject(const boost::uuids::uuid& nodeId, const boost::uuids::uuid& projectId, const std::string& userId);
	void CreateNodeProject(const model::NodeProject& item, const std::string& userId);

	PagingResult<model::NodeProject> GetNodeProjectList(const std::string& userId, uint32_t start, uint32_t limit);
	PagingResult<model::NodeProject> GetNodeProjectListByNodeId(const std::string& userId, uint32_t start, uint32_t limit, const boost::uuids::uuid& nodeId);

	model::NodeGroup GetNodeGroup(const boost::uuids::uuid& nodeId, int groupId, const std::string& userId);
	void DeleteNodeGroup(const boost::uuids::uuid& nodeId, int groupId, const std::string& userId);
	void CreateNodeGroup(const model::NodeGroup& item, const std::string& userId);

	PagingResult<model::NodeGroup> GetNodeGroupList(const std::string& userId, uint32_t start, uint32_t limit);
	PagingResult<model::NodeGroup> GetNodeGroupListByNodeId(const std::string& userId, uint32_t start, uint32_t limit, const boost::uuids::uuid& nodeId);

	model::GroupUser GetGroupUser(int groupId, const std::string& userId, const std::string& headerUserId);
	void DeleteGroupUser(int groupId, const std::string& userId, const std::string& headerUserId);
	void CreateGroupUser(const model::GroupUser& item, const std::string& headerUserId);

	PagingResult<model::GroupUser> GetGroupUserList(const std::string& headerUserId, uint32_t start, uint32_t limit);
	PagingResult<model::GroupUser> GetGroupUserListByGroupId(const std::string& headerUserId, uint32_t start, uint32_t limit, int groupId);

private:
	Repository& _repo;
	int _itemsLimitForList;
	std::string _jsonSchemasPath;
};

} // namespace svetit::node
