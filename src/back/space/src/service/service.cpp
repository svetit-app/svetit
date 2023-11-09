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

std::vector<model::Space> Service::GetList(const std::string userId, unsigned int start, unsigned int limit)
{
	return _repo.Space().SelectByUserId(userId, start, limit);
}

std::vector<model::Space> Service::GetAvailableList(const std::string userId, unsigned int start, unsigned int limit)
{
	return _repo.Space().SelectAvailable(userId, start, limit);
}

int Service::GetCount(const std::string userId) {
	return _repo.Space().CountByUserId(userId);
}

int Service::GetAvailableCount(const std::string userId) {
	return _repo.Space().CountAvailable(userId);
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
	return _repo.SpaceLink().Select(start, limit);
}

int Service::GetLinksCount() {
	return _repo.SpaceLink().Count();
}

std::vector<model::SpaceUser> Service::GetUserList(std::string userId, std::string spaceId, unsigned int start, unsigned int limit)
{
	const auto spaceUuid = utils::BoostUuidFromString(spaceId);

	bool isUserInside = _repo.SpaceUser().IsUserInside(spaceUuid, userId);

	if (!isUserInside)
		throw errors::BadRequest{"Wrong params"};

	return _repo.SpaceUser().Get(spaceUuid, start, limit);
}

int Service::GetUserCount(std::string userId, std::string spaceId) {
	const auto spaceUuid = utils::BoostUuidFromString(spaceId);

	bool isUserInside = _repo.SpaceUser().IsUserInside(spaceUuid, userId);

	if (!isUserInside)
		throw errors::BadRequest{"Wrong params"};

	return _repo.SpaceUser().CountBySpaceId(spaceUuid);
}

bool Service::isSpaceExistsByKey(std::string key) {
	return _repo.Space().IsExists(key);
}

bool Service::isCanCreate() {
	return _canCreate;
}

int Service::CountInvitationAvailable(const std::string currentUserId) {
	return _repo.SpaceInvitation().GetAvailableCount(currentUserId);
}

bool Service::CheckKeyByRegex(std::string key) {
	std::string regex = "[a-z0-9_]*";
	static const std::regex rx(regex);
	return std::regex_match(key.c_str(),rx);
}

bool Service::Create(std::string name, std::string key, bool requestsAllowed, std::string userId) {
	// check for key validity
	if (key == "u" || key == "auth" || key == "settings" || key == "main" || key == "api") {
		return false;
	}

	// check for key validity 2 stage
	if (key != userId) {
		// check is key valid UUID
		static const std::regex e("[a-f0-9]{8}-[a-f0-9]{4}-[a-f0-9]{4}-[a-f0-9]{4}-[a-f0-9]{12}");
   	 	if (std::regex_match(key, e)) {
	 		return false;
		} else {
			if (!CheckKeyByRegex(key)) {
				return false;
			}
		}
	}

	// check creation timeout

	if (!_repo.Space().IsReadyForCreationByTime(userId)) {
		return false;
	}

	// check for spaces limit
	const auto spacesCountForUser = _repo.Space().GetCountSpacesWithUser(userId);
	if (spacesCountForUser >= _spacesLimitForUser) {
		return false;
	}

	// creating space
	const auto spaceUuid = boost::uuids::random_generator()();

	const auto p1 = std::chrono::system_clock::now();
	const auto now = std::chrono::duration_cast<std::chrono::seconds>(p1.time_since_epoch()).count();
	//todo - is need to check that space with spaceUuis exists?
	_repo.Space().Insert(spaceUuid, name, key, requestsAllowed, now);

	//todo - is need to check that space with spaceUuis and user with userUuid exists?
	_repo.SpaceUser().Insert(spaceUuid, userId, true, now, "admin");
	return true;
}

bool Service::Delete(std::string id, std::string userId) {
	const auto spaceUuid = utils::BoostUuidFromString(id);

	const auto isOwner = _repo.SpaceUser().IsOwner(spaceUuid, userId);

	if (!isOwner) {
		return false;
	}

	const auto success = _repo.Space().Delete(spaceUuid);

	_repo.SpaceUser().DeleteBySpace(spaceUuid);
	_repo.SpaceInvitation().DeleteBySpace(spaceUuid);
	_repo.SpaceLink().DeleteBySpace(spaceUuid);

	return success;
}

bool Service::ValidateRole(std::string role) {
	if (role == "admin" || role == "user" || role == "guest")
		return true;
	return false;
}

bool Service::Invite(std::string creatorId, std::string spaceId, std::string userId, std::string role) {
	const auto spaceUuid = utils::BoostUuidFromString(spaceId);

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
		// todo - is need to check that space and users with spaceUuid, userId, creatorId exist?
		const auto p1 = std::chrono::system_clock::now();
		const auto now = std::chrono::duration_cast<std::chrono::seconds>(p1.time_since_epoch()).count();
		_repo.SpaceInvitation().Insert(spaceUuid, userId, role, creatorId, now);
	} else {
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
		if (!invitation.spaceId.is_nil() && !invitation.userId.empty()) {
			if (_repo.SpaceInvitation().DeleteById(id)){
				// todo - is it ok to use guest role if no role was set in invitation? is it possible to invitation exists with no role set in space_invitation table? may be for case when "I want to join"?
				const auto p1 = std::chrono::system_clock::now();
				const auto now = std::chrono::duration_cast<std::chrono::seconds>(p1.time_since_epoch()).count();
				_repo.SpaceUser().Insert(invitation.spaceId, invitation.userId, false, now, invitation.role.empty() ? "guest" : invitation.role);
				return true;
			}
		}
	}
	return false;
}

bool Service::DeleteInvitation(const int id) {
	return _repo.SpaceInvitation().DeleteById(id);
}

bool Service::CheckExpiredAtValidity(int64_t expiredAt) {
	// todo - is it right way to compare timestamps in this current situation?
	const auto p1 = std::chrono::system_clock::now();
	const auto now = std::chrono::duration_cast<std::chrono::seconds>(p1.time_since_epoch()).count();
	return (expiredAt > now);
}

void Service::CreateInvitationLink(const std::string spaceId, const std::string creatorId, const std::string name, const int64_t expiredAt) {
	// is need to check, that spaceId exists? creatorId exists?
	const auto p1 = std::chrono::system_clock::now();
	const auto now = std::chrono::duration_cast<std::chrono::seconds>(p1.time_since_epoch()).count();
	_repo.SpaceLink().Insert(
		boost::uuids::random_generator()(),
		utils::BoostUuidFromString(spaceId),
		creatorId,
		name,
		now,
		expiredAt
	);
}

bool Service::DeleteInvitationLink(const std::string id) {
	return _repo.SpaceLink().DeleteById(utils::BoostUuidFromString(id));
}

model::Space Service::GetById(std::string id, bool& found, std::string userId) {
	const auto space = _repo.Space().SelectById(utils::BoostUuidFromString(id), found);
	if (found) {
		const auto isUserInside = _repo.SpaceUser().IsUserInside(space.id, userId);
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
		const auto isUserInside = _repo.SpaceUser().IsUserInside(space.id, userId);
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

bool Service::InviteByLink(std::string creatorId, std::string link) {
	// todo - is some business logic needed for invitation by link like it was for invitation by login?
	bool found = false;
	model::SpaceLink linkEntity = _repo.SpaceLink().SelectById(utils::BoostUuidFromString(link), found);
	if (found) {
		const auto p1 = std::chrono::system_clock::now();
		const auto now = std::chrono::duration_cast<std::chrono::seconds>(p1.time_since_epoch()).count();
		if (linkEntity.expiredAt > now){
			const auto spaceUuid = linkEntity.spaceId;
			const auto userId = creatorId;
			const auto role = "";
			const auto createdAt = now;
			// todo - how to check for duplicates here (invitation that already was inserted)?
			_repo.SpaceInvitation().Insert(spaceUuid, userId, role, creatorId, createdAt);
			return true;
		} else {
			return false;
		}
	} else {
		return false;
	}
}

bool Service::DeleteUser(std::string requestUserId, std::string spaceId, std::string userId) {
	const auto spaceUuid = utils::BoostUuidFromString(spaceId);
	bool found = false;
	const auto user = _repo.SpaceUser().GetByIds(spaceUuid, userId, found);

	if (found) {
		if (user.isOwner) {
			return false;
		}

		if (user.userId == requestUserId) {
			return _repo.SpaceUser().Delete(spaceUuid, userId);
		} else {
			const auto isRequestUserAdmin = _repo.SpaceUser().IsAdmin(spaceUuid, requestUserId);
			if (isRequestUserAdmin) {
				return _repo.SpaceUser().Delete(spaceUuid, userId);
			}
		}
	}

	return false;
}

bool Service::UpdateUser(bool isRoleMode, std::string role, bool isOwnerMode, bool isOwner, std::string spaceId, std::string userId, std::string headerUserId) {
	const boost::uuids::uuid spaceUuid = utils::BoostUuidFromString(spaceId);

	bool foundHeaderUser = false;
	const auto headerUser = _repo.SpaceUser().GetByIds(spaceUuid, headerUserId, foundHeaderUser);
	if (!foundHeaderUser){
		return false;
	}

	bool foundUser = false;
	const auto user = _repo.SpaceUser().GetByIds(spaceUuid, userId, foundUser);
	if (!foundUser) {
		return false;
	}

	if (isRoleMode && !isOwnerMode) {
		if (headerUser.role == "admin") {
			if (user.userId != headerUser.userId) {
				if (!user.isOwner) {
					model::SpaceUser newUser;
					newUser.isOwner = user.isOwner;
					newUser.joinedAt = user.joinedAt;
					newUser.spaceId = user.spaceId;
					newUser.userId = user.userId;
					newUser.role = role;
					if (_repo.SpaceUser().Update(newUser)) {
						return true;
					} else {
						return false;
					}
				}
			}
		}
	}

	if (isOwnerMode) {
		if (!isOwner) {
			return false;
		}
		if (user.isOwner) {
			return false;
		}
		if (!headerUser.isOwner) {
			return false;
		}

		model::SpaceUser newUser = user;
		model::SpaceUser newHeaderUser = headerUser;

		newUser.isOwner = true;
		newUser.role = "admin";
		newHeaderUser.isOwner = false;

		// todo - rewrite this to one method call, something like _repo.SpaceUser().TransferOwnership() to exec everything in 1 transaction
		if (!_repo.SpaceUser().Update(newUser)) {
			return false;
		}

		if (!_repo.SpaceUser().Update(newHeaderUser)) {
			return false;
		}
		return true;
	}
	return false;
}

} // namespace svetit::space