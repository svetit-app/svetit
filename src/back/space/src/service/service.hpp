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
	bool IsListLimit(int limit);
	PagingResult<model::Space> GetList(const std::string& userId, unsigned int start, unsigned int limit);
	PagingResult<model::Space> GetAvailableList(const std::string& userId, unsigned int start, unsigned int limit);
	PagingResult<model::Space> GetAvailableListBySpaceName(const std::string& spaceName, const std::string& userId, unsigned int start, unsigned int limit);
	int GetAvailableCount(const std::string& userId);
	PagingResult<model::SpaceInvitation> GetInvitationList(unsigned int start, unsigned int limit, const std::string& userId);
	PagingResult<model::SpaceInvitation> GetInvitationListBySpaceForSpaceDetail(const boost::uuids::uuid& spaceId, unsigned int start, unsigned int limit, const std::string& userId);
	PagingResult<model::SpaceLink> GetLinkList(unsigned int start, unsigned int limit, const std::string& userId);
	PagingResult<model::SpaceLink> GetLinkListBySpace(const boost::uuids::uuid& spaceId, unsigned int start, unsigned int limit, const std::string& userId);
	PagingResult<model::SpaceUser> GetUserList(const std::string& userId, const boost::uuids::uuid& spaceId, unsigned int start, unsigned int limit);
	bool isSpaceExistsByKey(const std::string& key);
	bool isCanCreate();
	int CountInvitationAvailable(const std::string& userId);
	bool KeyCreateCheck(const std::string& key, const std::string& userId);
	bool KeyWeakCheck(const std::string& key);
	bool IsUserTimeouted(const std::string& userId);
	bool IsLimitReached(const std::string& userId);
	void Create(const std::string& name, const std::string& key, bool requestsAllowed, const std::string& userId);
	void Delete(const boost::uuids::uuid& id);
	bool IsSpaceOwner(const boost::uuids::uuid& id, const std::string& userId);
	void Invite(const std::string& creatorId, const boost::uuids::uuid& spaceId, const std::string& userId, const Role::Type& role);
	void ChangeRoleInInvitation(int id, const Role::Type& role, const std::string& userId);
	void ApproveInvitation(int id, const std::string& headerUserId);
	void DeleteInvitation(int id, const std::string& headerUserId);
	bool CheckExpiredAtValidity(int64_t expiredAt);
	void CreateInvitationLink(const boost::uuids::uuid& spaceId, const std::string& creatorId, const std::string& name, int64_t expiredAt);
	void DeleteInvitationLink(const boost::uuids::uuid& id, const std::string& userId);
	model::Space GetById(const boost::uuids::uuid& id, const std::string& userId);
	model::Space GetByKey(const std::string& key, const std::string& userId);
	model::Space GetByLink(const boost::uuids::uuid& link);
	bool InviteByLink(const std::string& creatorId, const boost::uuids::uuid& link);
	void DeleteUser(const boost::uuids::uuid& spaceId, const std::string& userId, const std::string& headerUserId);
	bool UpdateUser(const model::SpaceUser& updUser, const std::string& headerUserId);

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