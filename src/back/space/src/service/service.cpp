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

std::vector<svetit::space::model::Space> Service::GetList(unsigned int start, unsigned int limit)
{
	return _repo.Space().Select(start,limit);
}

int Service::GetCount() {
	return _repo.Space().Count();
}

std::vector<svetit::space::model::SpaceInvitation> Service::GetInvitationList(unsigned int start, unsigned int limit)
{
	return _repo.SpaceInvitation().Select(start,limit);
}

int Service::GetInvitationsCount() {
	return _repo.SpaceInvitation().Count();
}

std::vector<svetit::space::model::SpaceLink> Service::GetLinkList(unsigned int start, unsigned int limit)
{
	return _repo.SpaceLink().Select(start,limit);
}

int Service::GetLinksCount() {
	return _repo.SpaceLink().Count();
}

std::vector<svetit::space::model::SpaceUser> Service::GetUserList()
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

bool Service::Create(std::string name, std::string key, bool requestsAllowed, std::string userId, std::string& msg) {
	// check for name validity
	if (name == "u") {
		msg = "Restricted to create Space with 'u' name";
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

	_repo.Space().Insert(spaceUuid, name, key, requestsAllowed, std::chrono::system_clock::now());

	_repo.SpaceUser().Insert(spaceUuid, userUuid, true, std::chrono::system_clock::now(), "admin", requestsAllowed);
	return true;
}

void Service::Delete(std::string id) {
	const auto spaceUuid = utils::BoostUuidFromString(id);
	_repo.Space().Delete(spaceUuid);
	_repo.SpaceUser().DeleteBySpace(spaceUuid);
	_repo.SpaceInvitation().DeleteBySpace(spaceUuid);
	_repo.SpaceLink().DeleteBySpace(spaceUuid);
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

bool Service::Invite(std::string creatorId, std::string spaceId, std::string userId, std::string role, std::string msg) {
	const auto creatorUuid = utils::BoostUuidFromString(creatorId);
	const auto spaceUuid = utils::BoostUuidFromString(spaceId);
	const auto userUuid = utils::BoostUuidFromString(userId);

	bool isPossibleToInvite = false;

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

	if (true) {
		_repo.SpaceInvitation().Insert(spaceUuid, userUuid, role, creatorUuid, std::chrono::system_clock::now());
	} else {
		msg = "Can't create invitation because of business logic";
		return false;
	}

	return true;
}


} // namespace svetit::space