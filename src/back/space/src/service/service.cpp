#include "service.hpp"

#include <regex>
#include <boost/uuid/uuid_generators.hpp>
#include <boost/crc.hpp>

#include "../repo/repository.hpp"
#include <shared/errors.hpp>
#include <shared/paging.hpp>

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
  token-expire-secs:
    type: string
    description: How long token lives in seconds
  json-schemas-path:
    type: string
    description: Path to folder with JSON schemas
)");
}

Service::Service(
		const components::ComponentConfig& conf,
		const components::ComponentContext& ctx)
	: components::LoggableComponentBase{conf, ctx}
	, _repo{ctx.FindComponent<RepositoryComponent>()}
	, _canCreate{conf["can-create"].As<bool>()}
	, _defaultSpace{conf["default-space"].As<std::string>()}
	, _spacesLimitForUser{conf["spaces-limit-for-user"].As<int>()}
	, _itemsLimitForList{conf["items-limit-for-list"].As<int>()}
	, _tokenExpireSecs{conf["token-expire-secs"].As<int>()}
	, _tokens{ctx.FindComponent<tokens::Tokens>()}
	, _jsonSchemasPath{conf["json-schemas-path"].As<std::string>()}
{}

PagingResult<model::Space> Service::GetList(const std::string& userId, uint32_t start, uint32_t limit)
{
	if (!_defaultSpace.empty()) {
		const auto defSpace = _repo.Space().SelectByKey(_defaultSpace);
		if (!_repo.SpaceUser().IsUserInside(defSpace.id, userId)) {
			_repo.SpaceUser().Create(defSpace.id, userId, false, Role::Type::User);
		}
	}

	return _repo.SelectByUserId(userId, start, limit);
}

PagingResult<model::Space> Service::GetAvailableList(const std::string& userId, uint32_t start, uint32_t limit)
{
	return _repo.SelectAvailable(userId, start, limit);
}

PagingResult<model::Space> Service::GetAvailableListBySpaceName(const std::string& spaceName, const std::string& userId, uint32_t start, uint32_t limit)
{
	return _repo.SelectAvailableBySpaceName(spaceName, userId, start, limit);
}

PagingResult<model::SpaceInvitation> Service::GetInvitationList(uint32_t start, uint32_t limit, const std::string& userId)
{
	return _repo.SelectInvitations(userId, start, limit);
}

PagingResult<model::SpaceInvitation> Service::GetInvitationListBySpace(const boost::uuids::uuid& spaceId, uint32_t start, uint32_t limit, const std::string& userId)
{
	if (!_repo.SpaceUser().IsAdmin(spaceId, userId))
		throw errors::Forbidden403();

	return _repo.SelectInvitationsBySpace(spaceId, userId, start, limit);
}

PagingResult<model::SpaceLink> Service::GetLinkList(uint32_t start, uint32_t limit, const std::string& userId)
{
	return _repo.SelectSpaceLinkList(userId, start, limit);
}

PagingResult<model::SpaceLink> Service::GetLinkListBySpace(const boost::uuids::uuid& spaceId, uint32_t start, uint32_t limit, const std::string& userId)
{
	if (!_repo.SpaceUser().IsUserInside(spaceId, userId))
		throw errors::Forbidden403();

	return _repo.SpaceLink().SelectBySpace(spaceId, start, limit);
}

PagingResult<model::SpaceUser> Service::GetUserList(const std::string& userId, const boost::uuids::uuid& spaceId, uint32_t start, uint32_t limit)
{
	bool isUserInside = _repo.SpaceUser().IsUserInside(spaceId, userId);
	if (!isUserInside)
		throw errors::NotFound404{};

	return _repo.SpaceUser().Get(spaceId, start, limit);
}

bool Service::isSpaceExistsByKey(const std::string& key) {
	return _repo.Space().IsExists(key);
}

bool Service::isCanCreate() {
	return _canCreate;
}

int Service::CountInvitationAvailable(const std::string& currentUserId) {
	return _repo.GetAvailableInvitationsCount(currentUserId);
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
	auto trx = _repo.WithTrx();
	auto spaceId = trx.Space().Create(name, key, requestsAllowed);
	trx.SpaceUser().Create(spaceId, userId, /*isOwner*/true, Role::Admin);
	trx.Commit();
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
	_repo.CreateInvitation(spaceId, userId, role, creatorId);
}

void Service::ChangeRoleInInvitation(int id, const Role::Type& role, const std::string& userId) {
	const auto invitation = _repo.SpaceInvitation().SelectById(id);
	if (!_repo.SpaceUser().IsAdmin(invitation.spaceId, userId))
		throw errors::Forbidden403();

	_repo.SpaceInvitation().UpdateRole(id, role);
}

void Service::ApproveInvitation(int id, const std::string& headerUserId) {
	model::SpaceInvitation invitation = _repo.SpaceInvitation().SelectById(id);

	static const std::set<Role::Type> valid_roles{
		Role::User, Role::Guest, Role::Admin
	};
	if (!valid_roles.contains(invitation.role))
		throw errors::BadRequest400("Wrong role");

	// Я прошусь/хочет к нам - creatorId == userId
	if (invitation.creatorId == invitation.userId) {
		if (invitation.userId == headerUserId)
			throw errors::Forbidden403();
		// Я прошусь/хочет к нам - может одобрить только админ
		if (!_repo.SpaceUser().IsAdmin(invitation.spaceId, headerUserId))
			throw errors::Forbidden403();
	}
	// Меня/мы пригласили - может одобрить только пользователь которого пригласили
	else if (invitation.userId != headerUserId)
		throw errors::Forbidden403();

	_repo.SpaceInvitation().DeleteById(id);

	_repo.SpaceUser().Create(invitation.spaceId, invitation.userId, false, invitation.role);
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
		throw errors::Forbidden403();

	_repo.SpaceInvitation().DeleteById(id);
}

bool Service::CheckExpiredAtValidity(const std::chrono::system_clock::time_point& expiredAt) {
	// todo - is it right way to compare timestamps in this current situation?
	return expiredAt > std::chrono::system_clock::now();
}

void Service::CreateInvitationLink(const boost::uuids::uuid& spaceId, const std::string& creatorId, const std::string& name, const std::chrono::system_clock::time_point& expiredAt) {
	if (!_repo.SpaceUser().IsAdmin(spaceId, creatorId))
		throw errors::Forbidden403();

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
		throw errors::Forbidden403();

	_repo.SpaceLink().DeleteById(id);
}

model::Space Service::GetById(const boost::uuids::uuid& id, const std::string& userId) {
	const auto space = _repo.Space().SelectById(id);
	if (space.requestsAllowed)
		return space;

	if (!_repo.SpaceUser().IsUserInside(space.id, userId))
		throw errors::NotFound404{};
	return space;
}

model::Space Service::GetByKey(const std::string& key, const std::string& userId) {
	const auto space = _repo.Space().SelectByKey(key);
	if (space.requestsAllowed)
		return space;

	if (!_repo.SpaceUser().IsUserInside(space.id, userId))
		throw errors::NotFound404{};
	return space;
}

model::Space Service::GetByLink(const boost::uuids::uuid& link) {
	return _repo.SelectByLink(link);
}


bool Service::InviteByLink(const std::string& creatorId, const boost::uuids::uuid& linkId) {
	const auto link = _repo.SpaceLink().SelectById(linkId);

	const auto now = std::chrono::system_clock::now();
	if (link.expiredAt <= now)
		return false;
	_repo.CreateInvitation(link.spaceId, creatorId, Role::Type::Unknown, creatorId);
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
		auto trx = _repo.WithTrx();
		trx.SpaceUser().SetIsOwner(updUser.spaceId, caller.userId, /*isOwner*/ false);
		trx.SpaceUser().SetIsOwner(updUser.spaceId, updUser.userId, /*isOwner*/ true);
		trx.Commit();
		return true;
	}

	user.role = updUser.role;
	_repo.SpaceUser().Update(user);
	return true;
}

const std::string& Service::GetJSONSchemasPath() {
	return _jsonSchemasPath;
}

bool Service::isKeyReserved(const std::string& key) {
	static const std::set<std::string> reserved{
		"u", "auth", "settings", "main", "api"
	};
	return reserved.contains(key);
}

tokens::Tokens& Service::Tokens() {
	return _tokens;
}

model::Space Service::GetByKeyIfAdmin(const std::string& key, const std::string userId) {
	model::Space space = _repo.Space().SelectByKey(key);
	if (_repo.SpaceUser().IsAdmin(space.id, userId))
		return space;
	throw errors::Forbidden403();
}

std::string Service::GetKeyFromHeader(const std::string& header) {
	static std::regex rgx("^/([^/]+)/");
	std::smatch match;

	if (!std::regex_search(header.begin(), header.end(), match, rgx))
		throw errors::BadRequest400("Space key is missing");

	return match[1];
}

std::string Service::GenerateCookieName(const std::string& key) {
	uint32_t crc32 = generateCRC32(key);
	std::string cookieName = "space_" + std::to_string(crc32);
	return cookieName;
}

std::string Service::CreateToken(const std::string& id, const std::string& key, const std::string& userId, const std::string& role) {
	std::string token = Tokens().Create(key, id, role, userId, _tokenExpireSecs);
	return token;
}

model::Group Service::GetGroup(int id, const std::string& userId) {
	return _repo.Group().Select(id);
}

void Service::DeleteGroup(int id, const std::string& userId) {
	_repo.Group().Delete(id);
}

void Service::CreateGroup(const model::Group& item, const std::string& userId) {
	_repo.Group().Create(item);
}

void Service::UpdateGroup(const model::Group& item, const std::string& userId) {
	_repo.Group().Update(item);
}

PagingResult<model::Group> Service::GetGroupList(const std::string& userId, uint32_t start, uint32_t limit) {
	return _repo.Group().SelectList(start, limit);
}

PagingResult<model::Group> Service::GetGroupListBySpaceId(const std::string& userId, uint32_t start, uint32_t limit, const boost::uuids::uuid& spaceId) {
	return _repo.Group().SelectListBySpaceId(start, limit, spaceId);
}

uint32_t Service::generateCRC32(const std::string& data) {
	boost::crc_32_type result;
	result.process_bytes(data.data(), data.length());
	return result.checksum();
}

} // namespace svetit::space
