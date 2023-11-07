#include "service.hpp"

#include "../repo/repository.hpp"

#include <chrono>
#include <limits>
#include <stdexcept>
#include <cstdlib>
#include <ctime>
#include <userver/yaml_config/merge_schemas.hpp>
#include <userver/clients/http/component.hpp>
#include "userver/components/component_config.hpp"
#include "userver/components/component_context.hpp"
#include "userver/formats/json/serialize.hpp"
#include "userver/http/url.hpp"
#include <userver/crypto/base64.hpp>

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

std::vector<model::Space> Service::GetList(unsigned int start, unsigned int limit)
{
	return _repo.Space().Select(start,limit);
}

std::vector<model::Space> Service::GetAvailableList(const std::string userId, unsigned int start, unsigned int limit)
{
	return _repo.Space().SelectAvailable(utils::BoostUuidFromString(userId), start, limit);
}

int Service::GetCount() {
	return _repo.Space().Count();
}

int Service::GetAvailableCount(const std::string userId) {
	return _repo.Space().CountAvailable(utils::BoostUuidFromString(userId));
}

std::vector<model::SpaceInvitation> Service::GetInvitationList(unsigned int start, unsigned int limit)
{
	return _repo.SpaceInvitation().Select(start,limit);
}

int Service::GetInvitationsCount() {
	return _repo.SpaceInvitation().Count();
}

std::vector<model::SpaceLink> Service::GetLinkList(unsigned int start, unsigned int limit)
{
	return _repo.SpaceLink().Select(start,limit);
}

int Service::GetLinksCount() {
	return _repo.SpaceLink().Count();
}

std::vector<model::SpaceUser> Service::GetUserList()
{
	return {};
}

int Service::GetUsersCount() {
	return 0;
}

bool Service::isSpaceExistsByKey(std::string key) {
	return _repo.Space().IsExists(key);
}

bool Service::isCanCreate() {
	return _canCreate;
}

int Service::CountInvitationAvailable(const boost::uuids::uuid currentUserId) {
	return _repo.SpaceInvitation().GetAvailableCount(currentUserId);
}

bool Service::CheckKeyByRegex(std::string key) {
	std::string regex = "[a-z0-9_]*";
	std::regex rx(regex);
	return std::regex_match(key.c_str(),rx);
}

bool Service::CheckLinkNameByRegex(std::string linkName) {
	std::string regex = "[a-z0-9_]*";
	std::regex rx(regex);
	return std::regex_match(linkName.c_str(),rx);
}

bool Service::Create(std::string name, std::string key, bool requestsAllowed, std::string userId, std::string& msg) {
	// check for name validity
	if (key == "u" || key == "auth" || key == "settings" || key == "main" || key == "api") {
		msg = "Restricted to create Space with key '" + key + "'";
		return false;
	}

	//check for key validity (must be not valid uuid) not actual, because key is already checked before by Service::CheckKeyByRegex (regex [a-z0-9_]*)
   	/*
	static const std::regex e("[a-f0-9]{8}-[a-f0-9]{4}-[a-f0-9]{4}-[a-f0-9]{4}-[a-f0-9]{12}");
   	 if (std::regex_match(key, e)) {
	 	return false;
	}
	*/

	// check creation timeout
	const auto userUuid = utils::BoostUuidFromString(userId);

	if (!_repo.Space().IsReadyForCreationByTime(userUuid)) {
		msg = "Restricted to create space faster than 1 space in 1 minute for the same user";
		return false;
	}

	// check for spaces limit
	const auto spacesCountForUser = _repo.Space().GetCountSpacesWithUser(userUuid);
	if (spacesCountForUser >= _spacesLimitForUser) {
		msg = "Restricted to create Space when limit for spaces for user is reached";
		return false;
	}

	// creating space
	const auto spaceUuid = boost::uuids::random_generator()();

	//todo - is need to check that space with spaceUuis exists?
	_repo.Space().Insert(spaceUuid, name, key, requestsAllowed, std::chrono::system_clock::now());

	//todo - is need to check that space with spaceUuis and user with userUuid exists?
	_repo.SpaceUser().Insert(spaceUuid, userUuid, true, std::chrono::system_clock::now(), "admin");
	return true;
}

bool Service::Delete(std::string id, std::string userId) {
	const auto spaceUuid = utils::BoostUuidFromString(id);
	const auto userUuid = utils::BoostUuidFromString(userId);

	const auto isOwner = _repo.SpaceUser().IsOwner(spaceUuid, userUuid);

	if (!isOwner) {
		return false;
	}

	const auto success = _repo.Space().Delete(spaceUuid);

	_repo.SpaceUser().DeleteBySpace(spaceUuid);
	_repo.SpaceInvitation().DeleteBySpace(spaceUuid);
	_repo.SpaceLink().DeleteBySpace(spaceUuid);

	return success;
}

bool Service::ValidateUUID(std::string uuid) {
	static const std::regex e("[a-f0-9]{8}-[a-f0-9]{4}-[a-f0-9]{4}-[a-f0-9]{4}-[a-f0-9]{12}");
   	if (std::regex_match(uuid, e)) {
	 	return true;
	}
	return false;
}

bool Service::ValidateRole(std::string role) {
	if (role == "admin" || role == "user" || role == "guest")
		return true;
	return false;
}

bool Service::Invite(std::string creatorId, std::string spaceId, std::string userId, std::string role, std::string& msg) {
	const auto creatorUuid = utils::BoostUuidFromString(creatorId);
	const auto spaceUuid = utils::BoostUuidFromString(spaceId);
	const auto userUuid = utils::BoostUuidFromString(userId);

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

	// todo - true harcoded by now because isPossibleToInvite logic is not ready yet
	if (true) {
		// todo - is need to check that space and users with spaceUuid, userUuid, creatorUuid exist?
		_repo.SpaceInvitation().Insert(spaceUuid, userUuid, role, creatorUuid, std::chrono::system_clock::now());
	} else {
		msg = "Can't create invitation because of business logic";
		return false;
	}

	return true;
}

bool Service::ChangeRoleInInvitation(const int id, const std::string role) {
	return _repo.SpaceInvitation().UpdateRole(id, role);
}

bool Service::ApproveInvitation(const int id) {
	model::SpaceInvitation invitation;

	if (_repo.SpaceInvitation().SelectById(id, invitation)) {
		if (!invitation.spaceId.is_nil() && !invitation.userId.is_nil()) {
			if (_repo.SpaceInvitation().DeleteById(id)){
				// todo - is it ok to use guest role if no role was set in invitation? is it possible to invitation exists with no role set in space_invitation table? may be for case when "I want to join"?
				_repo.SpaceUser().Insert(invitation.spaceId, invitation.userId, false, std::chrono::system_clock::now(), invitation.role.empty() ? "guest" : invitation.role);
				return true;
			}
		}
	}
	return false;
}

bool Service::DeleteInvitation(const int id) {
	return _repo.SpaceInvitation().DeleteById(id);
}

bool Service::CheckExpiredAtValidity(std::chrono::system_clock::time_point expiredAt) {
	// todo - is it right way to compare timestamps in this current situation?
	return (expiredAt > std::chrono::system_clock::now());
}

void Service::CreateInvitationLink(const std::string spaceId, const std::string creatorId, const std::string name, const std::chrono::system_clock::time_point expiredAt) {
	// is need to check, that spaceId exists? creatorId exists?
	_repo.SpaceLink().Insert(
		boost::uuids::random_generator()(),
		utils::BoostUuidFromString(spaceId),
		utils::BoostUuidFromString(creatorId),
		name,
		std::chrono::system_clock::now(),
		expiredAt
	);
}

bool Service::DeleteInvitationLink(const std::string id) {
	return _repo.SpaceLink().DeleteById(utils::BoostUuidFromString(id));
}

model::Space Service::GetById(std::string id, bool& found, std::string userId) {
	const auto space = _repo.Space().SelectById(utils::BoostUuidFromString(id), found);
	if (found) {
		const auto isUserInside = _repo.SpaceUser().IsUserInside(space.id, utils::BoostUuidFromString(userId));
		if (isUserInside || space.requestsAllowed) {
			return space;
		} else {
			found = false;
		}
	}
	return {};
}

model::Space Service::GetByKey(std::string key, bool& found, std::string userId) {
	const auto space = _repo.Space().SelectByKey(key, found);
	if (found) {
		const auto isUserInside = _repo.SpaceUser().IsUserInside(space.id, utils::BoostUuidFromString(userId));
		if (isUserInside || space.requestsAllowed) {
			return space;
		} else {
			found = false;
		}
	}
	return {};
}

model::Space Service::GetByLink(std::string link, bool& found) {
	boost::uuids::uuid spaceUuid = _repo.SpaceLink().GetSpaceId(utils::BoostUuidFromString(link));
	if (!spaceUuid.is_nil()) {
		return _repo.Space().SelectById(spaceUuid, found);
	}
	return {};
}

bool Service::InviteByLink(std::string creatorId, std::string link, std::string& msg) {
	// todo - is some business logic needed for invitation by link like it was for invitation by login?
	bool found = false;
	model::SpaceLink linkEntity = _repo.SpaceLink().SelectById(utils::BoostUuidFromString(link), found);
	if (found) {
		if (linkEntity.expiredAt > std::chrono::system_clock::now()){
			const auto spaceUuid = linkEntity.spaceId;
			const auto userUuid = utils::BoostUuidFromString(creatorId);
			const auto role = "";
			const auto creatorUuid = utils::BoostUuidFromString(creatorId);
			const auto createdAt = std::chrono::system_clock::now();
			// todo - how to check for duplicates here (invitation that already was inserted)?
			_repo.SpaceInvitation().Insert(spaceUuid, userUuid, role, creatorUuid, createdAt);
			return true;
		} else {
			msg = "Link expired";
			return false;
		}
	} else {
		msg = "Link not found";
		return false;
	}
}

bool Service::DeleteUser(std::string requestUser, std::string spaceId, std::string userId) {
	const auto spaceUuid = utils::BoostUuidFromString(spaceId);
	const auto userUuid = utils::BoostUuidFromString(userId);
	const auto requestUserUuid = utils::BoostUuidFromString(requestUser);
	bool found = false;
	const auto user = _repo.SpaceUser().GetByIds(spaceUuid, userUuid, found);

	if (found) {
		if (user.isOwner) {
			return false;
		}

		if (user.userId == requestUser) {
			return _repo.SpaceUser().Delete(spaceUuid, userUuid);
		} else {
			const auto isRequestUserAdmin = _repo.SpaceUser().IsAdmin(spaceUuid, requestUserUuid);
			if (isRequestUserAdmin) {
				return _repo.SpaceUser().Delete(spaceUuid, userUuid);
			}
		}
	}

	return false;
}

} // namespace svetit::space