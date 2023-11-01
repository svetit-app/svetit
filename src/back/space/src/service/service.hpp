#pragma once

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

	std::vector<svetit::space::model::Space> GetList(unsigned int start, unsigned int limit);
	int GetCount();
	std::vector<svetit::space::model::SpaceInvitation> GetInvitationList(unsigned int start, unsigned int limit);
	int GetInvitationsCount();
	std::vector<svetit::space::model::SpaceLink> GetLinkList(unsigned int start, unsigned int limit);
	int GetLinksCount();
	std::vector<svetit::space::model::SpaceUser> GetUserList();
	int GetUsersCount();
	bool isSpaceExistsByKey(std::string key);
	bool isCanCreate();
	int CountInvitationAvailable(const boost::uuids::uuid userId);
	bool CheckKeyByRegex(std::string key);
	bool Create(std::string name, std::string key, bool requestsAllowed, std::string userId, std::string& msg);
	void Delete(std::string id);
	bool ValidateUUID(std::string uuid);
	bool ValidateRole(std::string role);
	bool Invite(std::string creatorId, std::string spaceId, std::string userId, std::string role, std::string msg);
	bool ChangeRoleInInvitation(const int id, const std::string role);
	bool ApproveInvitation(const int id);

private:
	std::vector<svetit::space::model::SpaceUser> _users;
	Repository& _repo;
	bool _canCreate;
	std::string _defaultSpace;
	int _spacesLimitForUser;
	int _itemsLimitForList;
};

} // namespace svetit::space
