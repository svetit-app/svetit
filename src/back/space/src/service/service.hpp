#pragma once

#include "../model/role.hpp"
#include "../model/space.hpp"
#include "../model/space_invitation.hpp"
#include "../model/space_link.hpp"
#include "../model/space_user.hpp"
#include "../model/errors.hpp"

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
	std::vector<model::Space> GetList(const std::string userId, unsigned int start, unsigned int limit);
	std::vector<model::Space> GetAvailableList(const std::string userId, unsigned int start, unsigned int limit);
	int GetCount(const std::string userId);
	int GetAvailableCount(const std::string userId);
	std::vector<model::SpaceInvitation> GetInvitationList(unsigned int start, unsigned int limit);
	int GetInvitationsCount();
	std::vector<model::SpaceLink> GetLinkList(unsigned int start, unsigned int limit);
	int GetLinksCount();
	std::vector<model::SpaceUser> GetUserList(std::string userId, std::string spaceId, unsigned int start, unsigned int limit);
	int GetUserCount(std::string userId, std::string spaceId);
	bool isSpaceExistsByKey(std::string key);
	bool isCanCreate();
	int CountInvitationAvailable(const std::string userId);
	bool CheckKeyByRegex(std::string key);
	bool Create(std::string name, std::string key, bool requestsAllowed, std::string userId);
	bool Delete(std::string id, std::string userId);
	bool ValidateRole(Role::Type role);
	bool Invite(std::string creatorId, std::string spaceId, std::string userId, Role::Type role);
	bool ChangeRoleInInvitation(const int id, const Role::Type role);
	bool ApproveInvitation(const int id);
	bool DeleteInvitation(const int id);
	bool CheckExpiredAtValidity(int64_t expiredAt);
	void CreateInvitationLink(const std::string spaceId, const std::string creatorId, const std::string name, const int64_t expiredAt);
	bool DeleteInvitationLink(const std::string id);
	model::Space GetById(std::string id, std::string userId);
	model::Space GetByKey(std::string key, std::string userId);
	model::Space GetByLink(std::string link);
	bool InviteByLink(std::string creatorId, std::string link);
	bool DeleteUser(std::string requestUser, std::string spaceId, std::string userId);
	bool UpdateUser(bool isRoleMode, Role::Type role, bool isOwnerMode, bool isOwner, std::string spaceId, std::string userId, std::string headerUserid);

private:
	std::vector<svetit::space::model::SpaceUser> _users;
	Repository& _repo;
	bool _canCreate;
	std::string _defaultSpace;
	int _spacesLimitForUser;
	int _itemsLimitForList;
};

} // namespace svetit::space