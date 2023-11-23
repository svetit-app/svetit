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

PagingResult<model::Space> Service::GetList(const std::string& userId, const unsigned int start, const unsigned int limit)
{
	if (!_defaultSpace.empty()) {
		const auto defSpace = _repo.Space().SelectByKey(_defaultSpace);
		if (!_repo.SpaceUser().IsUserInside(defSpace.id, userId)) {
			const auto nowDT = std::chrono::system_clock::now();
			const auto now = std::chrono::duration_cast<std::chrono::seconds>(nowDT.time_since_epoch()).count();
			// todo - what default role must be set here?
			_repo.SpaceUser().Insert(defSpace.id, userId, false, now, Role::Type::User);
		}
	}
	return _repo.Space().SelectByUserId(userId, start, limit);
}

PagingResult<model::Space> Service::GetAvailableList(const std::string& userId, const unsigned int start, const unsigned int limit)
{
	return _repo.Space().SelectAvailable(userId, start, limit);
}

std::vector<model::SpaceInvitation> Service::GetInvitationList(const unsigned int start, const unsigned int limit)
{
	return _repo.SpaceInvitation().Select(start,limit);
}

int Service::GetInvitationsCount() {
	return _repo.SpaceInvitation().Count();
}

std::vector<model::SpaceLink> Service::GetLinkList(const unsigned int start, const unsigned int limit)
{
	return _repo.SpaceLink().Select(start, limit);
}

int Service::GetLinksCount() {
	return _repo.SpaceLink().Count();
}

std::vector<model::SpaceUser> Service::GetUserList(const std::string& userId, const std::string& spaceId, const unsigned int start, const unsigned int limit)
{
	const auto spaceUuid = utils::BoostUuidFromString(spaceId);

	bool isUserInside = _repo.SpaceUser().IsUserInside(spaceUuid, userId);
	if (!isUserInside)
		throw errors::NotFound{};

	return _repo.SpaceUser().Get(spaceUuid, start, limit);
}

int Service::GetUserCount(const std::string& userId, const std::string& spaceId) {
	const auto spaceUuid = utils::BoostUuidFromString(spaceId);

	bool isUserInside = _repo.SpaceUser().IsUserInside(spaceUuid, userId);
	if (!isUserInside)
		throw errors::BadRequest{"Wrong params"};

	return _repo.SpaceUser().CountBySpaceId(spaceUuid);
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

bool Service::CheckKeyByRegex(const std::string& key) {
	std::string regex = "[a-z0-9_]*";
	static const std::regex rx(regex);
	return std::regex_match(key.c_str(),rx);
}

bool Service::IsKeyValid(const std::string& key) {
	if (key == "u" || key == "auth" || key == "settings" || key == "main" || key == "api")
		return false;
	return true;
}

bool Service::KeyAdditionalCheck(const std::string& key, const std::string& userId) {
	if (key != userId) {
		static const std::regex e("[a-f0-9]{8}-[a-f0-9]{4}-[a-f0-9]{4}-[a-f0-9]{4}-[a-f0-9]{12}");
		if (std::regex_match(key, e))
			return false;
		else if (!CheckKeyByRegex(key))
				return false;
	}
	return true;
}

bool Service::IsUserTimeouted(const std::string& userId) {
	if (!_repo.Space().IsReadyForCreationByTime(userId))
		return false;
	return true;
}

bool Service::IsLimitReached(const std::string& userId) {
	// check for spaces limit
	const auto spacesCountForUser = _repo.Space().GetCountSpacesWithUser(userId);
	if (spacesCountForUser < _spacesLimitForUser)
		return false;
	return true;
}

void Service::Create(const std::string& name, const std::string& key, bool requestsAllowed, const std::string& userId) {
	const auto spaceUuid = boost::uuids::random_generator()();
	const auto p1 = std::chrono::system_clock::now();
	const auto now = std::chrono::duration_cast<std::chrono::seconds>(p1.time_since_epoch()).count();
	//todo - is need to check that space with spaceUuis exists?
	_repo.Space().Insert(spaceUuid, name, key, requestsAllowed, now);

	//todo - is need to check that space with spaceUuis and user with userUuid exists?
	_repo.SpaceUser().Insert(spaceUuid, userId, true, now, Role::Type::Admin);
}

void Service::Delete(const std::string& id) {
	const auto spaceUuid = utils::BoostUuidFromString(id);

	_repo.Space().Delete(spaceUuid);
	_repo.SpaceUser().DeleteBySpace(spaceUuid);
	_repo.SpaceInvitation().DeleteBySpace(spaceUuid);
	_repo.SpaceLink().DeleteBySpace(spaceUuid);
}

bool Service::IsSpaceOwner(const std::string& id, const std::string& userId) {
	const auto spaceUuid = utils::BoostUuidFromString(id);
	return _repo.SpaceUser().IsOwner(spaceUuid, userId);
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
	// 		if (_repo.SpaceUser().IsAdminInSpace(creatorUuid, spaceUuid)){
	// 			if (!_repo.SpaceUser().IsInSpace(userUuid, spaceUuid)) {
	// 				isPossibleToInvite = true;
	// 			}
	// 		}
	// 	}
	// }

	const auto p1 = std::chrono::system_clock::now();
	const auto now = std::chrono::duration_cast<std::chrono::seconds>(p1.time_since_epoch()).count();
	_repo.SpaceInvitation().Insert(spaceUuid, userId, role, creatorId, now);
}

void Service::ChangeRoleInInvitation(const int id, const Role::Type& role) {
	_repo.SpaceInvitation().UpdateRole(id, role);
}

void Service::ApproveInvitation(const int id) {
	model::SpaceInvitation invitation = _repo.SpaceInvitation().SelectById(id);

	_repo.SpaceInvitation().DeleteById(id);
	// todo - is it ok to use guest role if no role was set in invitation? is it possible to invitation exists with no role set in space_invitation table? may be for case when "I want to join"?
	const auto p1 = std::chrono::system_clock::now();
	const auto now = std::chrono::duration_cast<std::chrono::seconds>(p1.time_since_epoch()).count();
	// todo - is it right role check after we moved to enum?
	_repo.SpaceUser().Insert(invitation.spaceId, invitation.userId, false, now, invitation.role ? Role::Type::Guest : invitation.role);
}

void Service::DeleteInvitation(const int id) {
	_repo.SpaceInvitation().DeleteById(id);
}

bool Service::CheckExpiredAtValidity(const int64_t expiredAt) {
	// todo - is it right way to compare timestamps in this current situation?
	const auto p1 = std::chrono::system_clock::now();
	const auto now = std::chrono::duration_cast<std::chrono::seconds>(p1.time_since_epoch()).count();
	return (expiredAt > now);
}

void Service::CreateInvitationLink(const boost::uuids::uuid& spaceId, const std::string& creatorId, const std::string& name, const int64_t expiredAt) {
	// is need to check, that spaceId exists? creatorId exists?
	const auto p1 = std::chrono::system_clock::now();
	const auto now = std::chrono::duration_cast<std::chrono::seconds>(p1.time_since_epoch()).count();
	_repo.SpaceLink().Insert(
		boost::uuids::random_generator()(),
		spaceId,
		creatorId,
		name,
		now,
		expiredAt
	);
}

void Service::DeleteInvitationLink(const std::string& id) {
	_repo.SpaceLink().DeleteById(utils::BoostUuidFromString(id));
}

model::Space Service::GetById(const std::string& id, const std::string& userId) {
	const auto space = _repo.Space().SelectById(utils::BoostUuidFromString(id));
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

model::Space Service::GetByLink(const std::string& link) {
	boost::uuids::uuid spaceUuid = _repo.SpaceLink().GetSpaceId(utils::BoostUuidFromString(link));
	if (!spaceUuid.is_nil())
		return _repo.Space().SelectById(spaceUuid);
	return {};
}

bool Service::IsLinkExpired(const std::string& link) {
	model::SpaceLink linkEntity = _repo.SpaceLink().SelectById(utils::BoostUuidFromString(link));
	const auto p1 = std::chrono::system_clock::now();
	const auto now = std::chrono::duration_cast<std::chrono::seconds>(p1.time_since_epoch()).count();
	if (linkEntity.expiredAt > now)
		return false;
	return true;
}

void Service::InviteByLink(const std::string& creatorId, const std::string& link) {
	// todo - is some business logic needed for invitation by link like it was for invitation by login?
	model::SpaceLink linkEntity = _repo.SpaceLink().SelectById(utils::BoostUuidFromString(link));
	const auto p1 = std::chrono::system_clock::now();
	const auto now = std::chrono::duration_cast<std::chrono::seconds>(p1.time_since_epoch()).count();
	const auto spaceUuid = linkEntity.spaceId;
	const auto userId = creatorId;
	const auto role = Role::Type::Unknown;
	const auto createdAt = now;
	// todo - how to check for duplicates here (invitation that already was inserted)?
	_repo.SpaceInvitation().Insert(spaceUuid, userId, role, creatorId, createdAt);
}

bool Service::CanDeleteUser(const std::string& requestUserId, const std::string& spaceId, const std::string& userId) {
	const auto spaceUuid = utils::BoostUuidFromString(spaceId);
	const auto user = _repo.SpaceUser().GetByIds(spaceUuid, userId);

	if (user.isOwner)
		return false;

	if (userId == requestUserId)
		return true;
	else {
		const auto isRequestUserAdmin = _repo.SpaceUser().IsAdmin(spaceUuid, requestUserId);
		if (isRequestUserAdmin)
			return true;
	}
	return false;
}

void Service::DeleteUser(const std::string& spaceId, const std::string& userId) {
	const auto spaceUuid = utils::BoostUuidFromString(spaceId);
	_repo.SpaceUser().Delete(spaceUuid, userId);
}

bool Service::CanUpdateUser(const bool isRoleMode, const bool isOwner, const boost::uuids::uuid& spaceUuid, const std::string& userId, const std::string& headerUserId) {
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

void Service::UpdateUser(const bool isRoleMode, const Role::Type& role, const bool isOwner, const boost::uuids::uuid& spaceUuid, const std::string& userId, const std::string& headerUserId) {
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

} // namespace svetit::space