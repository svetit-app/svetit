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

PagingResult<model::SpaceInvitation> Service::GetInvitationList(unsigned int start, unsigned int limit, const std::string& userId)
{
	return _repo.SelectInvitationsForSpaceList(start, limit, userId);
}

PagingResult<model::SpaceInvitation> Service::GetInvitationListBySpaceForSpaceDetail(const boost::uuids::uuid& spaceId, unsigned int start, unsigned int limit, const std::string& userId)
{
	if (!_repo.SpaceUser().IsAdmin(spaceId, userId))
		throw errors::Unauthorized();
	return _repo.SpaceInvitation().SelectBySpace(spaceId, start, limit);
}

PagingResult<model::SpaceLink> Service::GetLinkList(unsigned int start, unsigned int limit, const std::string& userId)
{
	return _repo.SelectSpaceLinkList(userId, start, limit);
}

PagingResult<model::SpaceLink> Service::GetLinkListBySpace(const std::string& spaceId, unsigned int start, unsigned int limit, const std::string& userId)
{
	if (!_repo.SpaceUser().IsUserInside(utils::BoostUuidFromString(spaceId), userId))
		throw errors::Unauthorized();

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
	_repo.CreateSpaceAndItsOwner(name, key, requestsAllowed, userId);
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

void Service::Invite(const std::string& creatorId, const boost::uuids::uuid& spaceId, const std::string& userId, const Role::Type& role) {
	_repo.SpaceInvitation().Insert(spaceId, userId, role, creatorId);
}

void Service::ChangeRoleInInvitation(int id, const Role::Type& role, const std::string& userId) {
	const auto invitation = _repo.SpaceInvitation().SelectById(id);
	if (!_repo.SpaceUser().IsAdmin(invitation.spaceId, userId))
		throw errors::Unauthorized();

	_repo.SpaceInvitation().UpdateRole(id, role);
}

void Service::ApproveInvitation(int id, const std::string& headerUserId) {
	// todo - rewrite into 1 transaction
	model::SpaceInvitation invitation = _repo.SpaceInvitation().SelectById(id);

	bool isEnoughRights = false;


	if (invitation.creatorId == invitation.userId && invitation.userId != headerUserId) {
		if (_repo.SpaceUser().IsAdmin(invitation.spaceId, headerUserId))
			isEnoughRights = true;
	} else if (invitation.creatorId != invitation.userId && invitation.userId == headerUserId)
			isEnoughRights = true;

	if (!isEnoughRights)
		throw errors::Unauthorized();

	static const std::set<std::string> valid_roles{
		"user", "guest", "admin"
	};
	if (!valid_roles.contains(Role::ToString(invitation.role)))
		throw errors::BadRequest("Wrong role");

	_repo.SpaceInvitation().DeleteById(id);

	_repo.SpaceUser().Insert(invitation.spaceId, invitation.userId, false, invitation.role);
}

void Service::DeleteInvitation(int id, const std::string& headerUserId) {
	const auto invitation = _repo.SpaceInvitation().SelectById(id);

	bool isEnoughRights = false;

	if (invitation.creatorId == invitation.userId && invitation.userId == headerUserId) {
		// I want to join
		isEnoughRights = true;
	} else if (_repo.SpaceUser().IsAdmin(invitation.spaceId, headerUserId)) {
		// other cases need admin rights
		isEnoughRights = true;
	}

	if (!isEnoughRights)
		throw errors::Unauthorized();

	_repo.SpaceInvitation().DeleteById(id);
}

bool Service::CheckExpiredAtValidity(int64_t expiredAt) {
	// todo - is it right way to compare timestamps in this current situation?
	const auto p1 = std::chrono::system_clock::now();
	const auto now = std::chrono::duration_cast<std::chrono::seconds>(p1.time_since_epoch()).count();
	return (expiredAt > now);
}

void Service::CreateInvitationLink(const boost::uuids::uuid& spaceId, const std::string& creatorId, const std::string& name, int64_t expiredAt) {
	if (!_repo.SpaceUser().IsAdmin(spaceId, creatorId))
		throw errors::Unauthorized();

	_repo.SpaceLink().Insert(
		spaceId,
		creatorId,
		name,
		expiredAt
	);
}

void Service::DeleteInvitationLink(const boost::uuids::uuid& id, const std::string& userId) {
	const auto link = _repo.SpaceLink().SelectById(id);

	if (!_repo.SpaceUser().IsAdmin(link.spaceId, userId))
		throw errors::Unauthorized();

	_repo.SpaceLink().DeleteById(id);
}

model::Space Service::GetById(const boost::uuids::uuid& id, const std::string& userId) {
	const auto space = _repo.Space().SelectById(id);
	if (space.requestsAllowed)
		return space;

	if (!_repo.SpaceUser().IsUserInside(space.id, userId))
		throw errors::NotFound{};
	return space;
}

model::Space Service::GetByKey(const std::string& key, const std::string& userId) {
	const auto space = _repo.Space().SelectByKey(key);
	if (space.requestsAllowed)
		return space;

	if (!_repo.SpaceUser().IsUserInside(space.id, userId))
		throw errors::NotFound{};
	return space;
}

model::Space Service::GetByLink(const boost::uuids::uuid& link) {
	return _repo.SelectByLink(link);
}


bool Service::InviteByLink(const std::string& creatorId, const boost::uuids::uuid& linkId) {
	const auto link = _repo.SpaceLink().SelectById(linkId);

	const auto p1 = std::chrono::system_clock::now();
	const auto now = std::chrono::duration_cast<std::chrono::seconds>(p1.time_since_epoch()).count();
	if (link.expiredAt > now)
		return false;
	_repo.SpaceInvitation().Insert(link.spaceId, creatorId, Role::Type::Unknown, creatorId);
	return true;
}

void Service::DeleteUser(const boost::uuids::uuid& spaceId, const std::string& userId, const std::string& headerUserId) {
	_repo.SpaceUser().Delete(spaceId, userId, headerUserId);
}

bool Service::UpdateUser(const model::SpaceUser& updUser, const std::string& headerUserId) {
	// Нет смысла менять что-то у самого себя:
	if (updUser.userId == headerUserId)
		return false;

	const auto caller = _repo.SpaceUser().GetByIds(updUser.spaceId, headerUserId);

	// Только админ может что-то менять
	if (caller.role != Role::Type::Admin)
		return false;

	// Только владелец может сменить владельца
	if (updUser.isOwner && !caller.isOwner)
		return false;

	auto user = _repo.SpaceUser().GetByIds(updUser.spaceId, updUser.userId);

	// У владельца ничего менять нельзя
	if (user.isOwner)
		return false;

	if (updUser.isOwner) {
		_repo.SpaceUser().TransferOwnership(updUser.spaceId, caller.userId, updUser.userId);
		return true;
	}

	user.role = updUser.role;
	_repo.SpaceUser().Update(user);
	return true;
}

bool Service::isKeyReserved(const std::string& key) {
	static const std::set<std::string> reserved{
		"u", "auth", "settings", "main", "api"
	};
	return reserved.contains(key);
}

} // namespace svetit::space