#pragma once

#include "../model/role.hpp"
#include "../model/space.hpp"
#include "../model/space_invitation.hpp"
#include "../model/space_link.hpp"
#include "../model/space_user.hpp"

#include <string>
#include <string_view>
#include <vector>
#include <regex>

#include <boost/uuid/uuid.hpp>
#include <boost/uuid/uuid_generators.hpp>
#include <boost/uuid/uuid_io.hpp>

#include <userver/components/loggable_component_base.hpp>
#include <userver/yaml_config/schema.hpp>
#include <userver/utest/using_namespace_userver.hpp>
#include <userver/http/url.hpp>

namespace svetit::space {

class Repository;

class Service final : public components::LoggableComponentBase {
public:
	static constexpr std::string_view kName = "main-service";
	static yaml_config::Schema GetStaticConfigSchema();

	explicit Service(
		const components::ComponentConfig& conf,
		const components::ComponentContext& ctx);
	std::vector<model::Space> GetList(const std::string& userId, const unsigned int start, const unsigned int limit);
	std::vector<model::Space> GetAvailableList(const std::string& userId, const unsigned int start, const unsigned int limit);
	int GetCount(const std::string& userId);
	int GetAvailableCount(const std::string& userId);
	std::vector<model::SpaceInvitation> GetInvitationList(const unsigned int start, const unsigned int limit);
	int GetInvitationsCount();
	std::vector<model::SpaceLink> GetLinkList(const unsigned int start, const unsigned int limit);
	int GetLinksCount();
	std::vector<model::SpaceUser> GetUserList(const std::string& userId, const std::string& spaceId, const unsigned int start, const unsigned int limit);
	int GetUserCount(const std::string& userId, const std::string& spaceId);
	bool isSpaceExistsByKey(const std::string& key);
	bool isCanCreate();
	int CountInvitationAvailable(const std::string& userId);
	bool CheckKeyByRegex(const std::string& key);
	bool Create(const std::string& name, const std::string& key, const bool requestsAllowed, const std::string& userId);
	bool Delete(const std::string& id, const std::string& userId);
	bool ValidateRole(const Role::Type& role);
	bool Invite(const std::string& creatorId, const std::string& spaceId, const std::string& userId, const Role::Type& role);
	bool ChangeRoleInInvitation(const int id, const Role::Type& role);
	bool ApproveInvitation(const int id);
	bool DeleteInvitation(const int id);
	bool CheckExpiredAtValidity(const int64_t expiredAt);
	void CreateInvitationLink(const std::string& spaceId, const std::string& creatorId, const std::string& name, const int64_t expiredAt);
	bool DeleteInvitationLink(const std::string& id);
	model::Space GetById(const std::string& id, const std::string& userId);
	model::Space GetByKey(const std::string& key, const std::string& userId);
	model::Space GetByLink(const std::string& link);
	bool InviteByLink(const std::string& creatorId, const std::string& link);
	bool DeleteUser(const std::string& requestUser, const std::string& spaceId, const std::string& userId);
	bool UpdateUser(const bool isRoleMode, const Role::Type& role, const bool isOwnerMode, const bool isOwner, const std::string& spaceId, const std::string& userId, const std::string& headerUserid);

private:
	std::vector<model::SpaceUser> _users;
	Repository& _repo;
	bool _canCreate;
	std::string _defaultSpace;
	int _spacesLimitForUser;
	int _itemsLimitForList;
};

} // namespace svetit::space