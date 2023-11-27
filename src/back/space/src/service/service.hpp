#pragma once

#include "../model/role.hpp"
#include "../model/space.hpp"
#include "../model/space_invitation.hpp"
#include "../model/space_link.hpp"
#include "../model/space_user.hpp"
#include "../../../shared/paging.hpp"

#include <userver/components/loggable_component_base.hpp>
#include <userver/yaml_config/schema.hpp>
#include <userver/utest/using_namespace_userver.hpp>
#include <userver/http/url.hpp>
#include <userver/utils/boost_uuid4.hpp>

namespace svetit::space {

class Repository;

class Service final : public components::LoggableComponentBase {
public:
	static constexpr std::string_view kName = "main-service";
	static yaml_config::Schema GetStaticConfigSchema();

	explicit Service(
		const components::ComponentConfig& conf,
		const components::ComponentContext& ctx);
	PagingResult<model::Space> GetList(const std::string& userId, const unsigned int start, const unsigned int limit);
	PagingResult<model::Space> GetAvailableList(const std::string& userId, const unsigned int start, const unsigned int limit);
	int GetAvailableCount(const std::string& userId);
	PagingResult<model::SpaceInvitation> GetInvitationList(const unsigned int start, const unsigned int limit);
	PagingResult<model::SpaceLink> GetLinkList(const unsigned int start, const unsigned int limit);
	PagingResult<model::SpaceUser> GetUserList(const std::string& userId, const boost::uuids::uuid& spaceId, const unsigned int start, const unsigned int limit);
	bool isSpaceExistsByKey(const std::string& key);
	bool isCanCreate();
	int CountInvitationAvailable(const std::string& userId);
	bool KeyCreateCheck(const std::string& key, const std::string& userId);
	bool KeyWeakCheck(const std::string& key);
	bool IsUserTimeouted(const std::string& userId);
	bool IsLimitReached(const std::string& userId);
	void Create(const std::string& name, const std::string& key, const bool requestsAllowed, const std::string& userId);
	void Delete(const boost::uuids::uuid& id);
	bool IsSpaceOwner(const boost::uuids::uuid& id, const std::string& userId);
	bool ValidateRole(const Role::Type& role);
	void Invite(const std::string& creatorId, const boost::uuids::uuid& spaceId, const std::string& userId, const Role::Type& role);
	void ChangeRoleInInvitation(const int id, const Role::Type& role);
	void ApproveInvitation(const int id);
	void DeleteInvitation(const int id);
	bool CheckExpiredAtValidity(const int64_t expiredAt);
	void CreateInvitationLink(const boost::uuids::uuid& spaceId, const std::string& creatorId, const std::string& name, const int64_t expiredAt);
	void DeleteInvitationLink(const boost::uuids::uuid& id);
	model::Space GetById(const boost::uuids::uuid& id, const std::string& userId);
	model::Space GetByKey(const std::string& key, const std::string& userId);
	model::Space GetByLink(const boost::uuids::uuid& link);
	bool IsLinkExpired(const boost::uuids::uuid& link);
	void InviteByLink(const std::string& creatorId, const boost::uuids::uuid& link);
	bool CanDeleteUser(const std::string& requestUserId, const boost::uuids::uuid& spaceId, const std::string& userId);
	void DeleteUser(const boost::uuids::uuid& spaceId, const std::string& userId);
	bool CanUpdateUser(const bool isRoleMode, const bool isOwner, const boost::uuids::uuid& spaceUuid, const std::string& userId, const std::string& headerUserId);
	void UpdateUser(const bool isRoleMode, const Role::Type& role, const bool isOwner, const boost::uuids::uuid& spaceId, const std::string& userId, const std::string& headerUserid);

private:
	std::vector<model::SpaceUser> _users;
	Repository& _repo;
	bool _canCreate;
	std::string _defaultSpace;
	int _spacesLimitForUser;
	int _itemsLimitForList;
	bool isKeyReserved(const std::string& key);
};

} // namespace svetit::space