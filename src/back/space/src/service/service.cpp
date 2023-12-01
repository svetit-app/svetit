#include "service.hpp"

#include <regex>
#include <boost/uuid/uuid_generators.hpp>

#include "../repo/repository.hpp"
#include "../../../shared/errors.hpp"
#include "../../../shared/paging.hpp"

#include <userver/yaml_config/merge_schemas.hpp>
#include "userver/components/component_config.hpp"
#include "userver/components/component_context.hpp"

namespace svetit::space {

/*static*/ yaml_config::Schema Service::GetStaticConfigSchema()
{
	return yaml_config::MergeSchemas<components::LoggableComponentBase>(R"(
type: object
description: Main service component
additionalProperties: false
properties:
  can-create:
    type: string
    description: Can create new spaces
  default-space:
    type: string
    description: Default space to join
  spaces-limit-for-user:
    type: string
    description: How many spaces user can join
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
	, _canCreate{conf["can-create"].As<bool>()}
	, _defaultSpace{conf["default-space"].As<std::string>()}
	, _spacesLimitForUser{conf["spaces-limit-for-user"].As<int>()}
	, _itemsLimitForList{conf["items-limit-for-list"].As<int>()}
{}

bool Service::IsListLimit(int limit) {
	return _itemsLimitForList < limit;
}

PagingResult<model::Space> Service::GetList(const std::string& userId, unsigned int start, unsigned int limit)
{
	if (!_defaultSpace.empty()) {
		const auto defSpace = _repo.Space().SelectByKey(_defaultSpace);
		if (!_repo.SpaceUser().IsUserInside(defSpace.id, userId)) {
			// todo - what default role must be set here?
			_repo.SpaceUser().Insert(defSpace.id, userId, false, Role::Type::User);
		}
	}
	return _repo.SelectByUserId(userId, start, limit);
}

PagingResult<model::Space> Service::GetAvailableList(const std::string& userId, unsigned int start, unsigned int limit)
{
	return _repo.SelectAvailable(userId, start, limit);
}

PagingResult<model::Space> Service::GetAvailableListBySpaceName(const std::string& spaceName, const std::string& userId, unsigned int start, unsigned int limit)
{
	return _repo.SelectAvailableBySpaceName(spaceName, userId, start, limit);
}

PagingResult<model::SpaceInvitation> Service::GetInvitationList(unsigned int start, unsigned int limit)
{
	return _repo.SpaceInvitation().Select(start, limit);
}

PagingResult<model::SpaceInvitation> Service::GetInvitationListBySpace(const std::string& spaceId, unsigned int start, unsigned int limit)
{
	return _repo.SpaceInvitation().SelectBySpace(utils::BoostUuidFromString(spaceId), start, limit);
}

PagingResult<model::SpaceLink> Service::GetLinkList(unsigned int start, unsigned int limit)
{
	return _repo.SpaceLink().Select(start, limit);
}

PagingResult<model::SpaceLink> Service::GetLinkListBySpace(const std::string& spaceId, unsigned int start, unsigned int limit)
{
	return _repo.SpaceLink().SelectBySpace(utils::BoostUuidFromString(spaceId), start, limit);
}

PagingResult<model::SpaceUser> Service::GetUserList(const std::string& userId, const boost::uuids::uuid& spaceId, unsigned int start, unsigned int limit)
{
	bool isUserInside = _repo.SpaceUser().IsUserInside(spaceId, userId);
	if (!isUserInside)
		throw errors::NotFound{};

	return _repo.SpaceUser().Get(spaceId, start, limit);
}

bool Service::isSpaceExistsByKey(const std::string& key) {
	return _repo.Space().IsExists(key);
}

bool Service::isCanCreate() {
	return _canCreate;
}

int Service::CountInvitationAvailable(const std::string& currentUserId) {
	return _repo.SpaceInvitation().GetAvailableCount(currentUserId);
}

bool Service::KeyCreateCheck(const std::string& key, const std::string& userId) {
	// При создании ключ может либо совпадать с userId
	if (key == userId)
		return true;

	// либо не должен быть зарезервирован
	if (isKeyReserved(key))
		return false;

	// и содержать только допустимые символы
	static const std::regex rx("[a-z0-9][a-z0-9_]+");
	return std::regex_match(key, rx);
}

bool Service::KeyWeakCheck(const std::string& key) {
	// При получении Space, ключ не должен быть зарезервирован
	if (isKeyReserved(key))
		return false;
	// и содержать ослабленный вариант допустимых символов
	static const std::regex rx("[a-z0-9_-]+");
	return std::regex_match(key, rx);
}

bool Service::IsUserTimeouted(const std::string& userId) {
	return _repo.IsReadyForCreationByTime(userId);
}

bool Service::IsLimitReached(const std::string& userId) {
	return _spacesLimitForUser <= _repo.GetCountSpacesWithUser(userId);
}

void Service::Create(const std::string& name, const std::string& key, bool requestsAllowed, const std::string& userId) {
	//todo - is need to check that space with spaceUuis exists?
	const auto spaceUuid = _repo.Space().Insert(name, key, requestsAllowed);

	//todo - is need to check that space with spaceUuis and user with userUuid exists?
	_repo.SpaceUser().Insert(spaceUuid, userId, true, Role::Type::Admin);
}

void Service::Delete(const boost::uuids::uuid& id) {
	_repo.SpaceUser().DeleteBySpace(id);
	_repo.SpaceInvitation().DeleteBySpace(id);
	_repo.SpaceLink().DeleteBySpace(id);
	_repo.Space().Delete(id);
}

bool Service::IsSpaceOwner(const boost::uuids::uuid& id, const std::string& userId) {
	return _repo.SpaceUser().IsOwner(id, userId);
}

// todo - need to get rid of this func?
bool Service::ValidateRole(const Role::Type& role) {
	if (role == Role::Type::Admin || role == Role::Type::User || role == Role::Type::Guest)
		return true;
	return false;
}

void Service::Invite(const std::string& creatorId, const boost::uuids::uuid& spaceUuid, const std::string& userId, const Role::Type& role) {

	// bool isPossibleToInvite = false;

	// if (!_repo.Space().IsRequestsAllowed(spaceUuid)) {
	// 	if (_repo.SpaceUser().IsAdminInSpace(creatorUuid, spaceUuid)) {
	// 		if (!_repo.SpaceUser().IsInSpace(userUuid, spaceUuid)) {
	// 			isPossibleToInvite = true;
	// 		}
	// 	}
	// } else {
	// 	if (!_repo.SpaceUser().IsInSpace(creatorUuid, spaceUuid)) {
	// 		if (creatorUuid == userUuid) {
	// 			if (!_repo.SpaceInvitation().IsExist(spaceUuid, creatorUuid, creatorUuid)) {
	// 				isPossibleToInvite = true;
	// 			}
	// 		}
	// 	} else {
	// 		if (_repo.SpaceUser().IsAdminInSpace(creatorUuid, spaceUuid)) {
	// 			if (!_repo.SpaceUser().IsInSpace(userUuid, spaceUuid)) {
	// 				isPossibleToInvite = true;
	// 			}
	// 		}
	// 	}
	// }
	_repo.SpaceInvitation().Insert(spaceUuid, userId, role, creatorId);
}

void Service::ChangeRoleInInvitation(int id, const Role::Type& role) {
	_repo.SpaceInvitation().UpdateRole(id, role);
}

void Service::ApproveInvitation(int id) {
	model::SpaceInvitation invitation = _repo.SpaceInvitation().SelectById(id);

	_repo.SpaceInvitation().DeleteById(id);
	// todo - is it ok to use guest role if no role was set in invitation? is it possible to invitation exists with no role set in space_invitation table? may be for case when "I want to join"?
	// todo - is it right role check after we moved to enum?
	_repo.SpaceUser().Insert(invitation.spaceId, invitation.userId, false, invitation.role ? Role::Type::Guest : invitation.role);
}

void Service::DeleteInvitation(int id) {
	_repo.SpaceInvitation().DeleteById(id);
}

bool Service::CheckExpiredAtValidity(int64_t expiredAt) {
	// todo - is it right way to compare timestamps in this current situation?
	const auto p1 = std::chrono::system_clock::now();
	const auto now = std::chrono::duration_cast<std::chrono::seconds>(p1.time_since_epoch()).count();
	return (expiredAt > now);
}

void Service::CreateInvitationLink(const boost::uuids::uuid& spaceId, const std::string& creatorId, const std::string& name, int64_t expiredAt) {
	// is need to check, that spaceId exists? creatorId exists?
	_repo.SpaceLink().Insert(
		spaceId,
		creatorId,
		name,
		expiredAt
	);
}

void Service::DeleteInvitationLink(const boost::uuids::uuid& id) {
	_repo.SpaceLink().DeleteById(id);
}

model::Space Service::GetById(const boost::uuids::uuid& id, const std::string& userId) {
	const auto space = _repo.Space().SelectById(id);
	const auto isUserInside = _repo.SpaceUser().IsUserInside(space.id, userId);
	if (isUserInside || space.requestsAllowed)
		return space;
	else
		throw errors::NotFound{};
	return {};
}

model::Space Service::GetByKey(const std::string& key, const std::string& userId) {
	const auto space = _repo.Space().SelectByKey(key);
	const auto isUserInside = _repo.SpaceUser().IsUserInside(space.id, userId);
	if (isUserInside || space.requestsAllowed)
		return space;
	else
		throw errors::NotFound{};
	return {};
}

model::Space Service::GetByLink(const boost::uuids::uuid& link) {
	boost::uuids::uuid spaceUuid = _repo.SpaceLink().GetSpaceId(link);
	if (!spaceUuid.is_nil())
		return _repo.Space().SelectById(spaceUuid);
	return {};
}

bool Service::IsLinkExpired(const boost::uuids::uuid& link) {
	model::SpaceLink linkEntity = _repo.SpaceLink().SelectById(link);
	const auto p1 = std::chrono::system_clock::now();
	const auto now = std::chrono::duration_cast<std::chrono::seconds>(p1.time_since_epoch()).count();
	if (linkEntity.expiredAt > now)
		return false;
	return true;
}

void Service::InviteByLink(const std::string& creatorId, const boost::uuids::uuid& link) {
	// todo - is some business logic needed for invitation by link like it was for invitation by login?
	model::SpaceLink linkEntity = _repo.SpaceLink().SelectById(link);
	const auto spaceUuid = linkEntity.spaceId;
	const auto userId = creatorId;
	const auto role = Role::Type::Unknown;
	// todo - how to check for duplicates here (invitation that already was inserted)?
	_repo.SpaceInvitation().Insert(spaceUuid, userId, role, creatorId);
}

bool Service::CanDeleteUser(const std::string& requestUserId, const boost::uuids::uuid& spaceId, const std::string& userId) {
	const auto user = _repo.SpaceUser().GetByIds(spaceId, userId);

	if (user.isOwner)
		return false;

	if (userId == requestUserId)
		return true;
	else {
		const auto isRequestUserAdmin = _repo.SpaceUser().IsAdmin(spaceId, requestUserId);
		if (isRequestUserAdmin)
			return true;
	}
	return false;
}

void Service::DeleteUser(const boost::uuids::uuid& spaceId, const std::string& userId) {
	_repo.SpaceUser().Delete(spaceId, userId);
}

bool Service::CanUpdateUser(bool isRoleMode, bool isOwner, const boost::uuids::uuid& spaceUuid, const std::string& userId, const std::string& headerUserId) {
	const auto headerUser = _repo.SpaceUser().GetByIds(spaceUuid, headerUserId);

	const auto user = _repo.SpaceUser().GetByIds(spaceUuid, userId);

	if (isOwner) {
		if (user.isOwner)
			return false;
		if (!headerUser.isOwner)
			return false;
		return true;
	} else {
		if (isRoleMode)
			if (headerUser.role == Role::Type::Admin)
				if (!user.isOwner)
					return true;
	}
	return false;
}

void Service::UpdateUser(bool isRoleMode, const Role::Type& role, bool isOwner, const boost::uuids::uuid& spaceUuid, const std::string& userId, const std::string& headerUserId) {
	const auto headerUser = _repo.SpaceUser().GetByIds(spaceUuid, headerUserId);

	const auto user = _repo.SpaceUser().GetByIds(spaceUuid, userId);

	if (isOwner) {
		model::SpaceUser newUser = user;
		model::SpaceUser newHeaderUser = headerUser;

		newUser.isOwner = true;
		newUser.role = Role::Type::Admin;
		newHeaderUser.isOwner = false;

		// todo - rewrite this to one method call, something like _repo.SpaceUser().TransferOwnership() to exec everything in 1 transaction
		_repo.SpaceUser().Update(newUser);

		_repo.SpaceUser().Update(newHeaderUser);
	} else if (isRoleMode) {
		model::SpaceUser newUser;
		newUser.isOwner = user.isOwner;
		newUser.joinedAt = user.joinedAt;
		newUser.spaceId = user.spaceId;
		newUser.userId = user.userId;
		newUser.role = role;
		_repo.SpaceUser().Update(newUser);
	}
}

bool Service::isKeyReserved(const std::string& key) {
	static const std::set<std::string> reserved{
		"u", "auth", "settings", "main", "api"
	};
	return reserved.contains(key);
}

} // namespace svetit::space