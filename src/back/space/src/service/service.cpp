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
#include <userver/utils/rand.hpp>

namespace svetit::space {

/*static*/ yaml_config::Schema Service::GetStaticConfigSchema()
{
	return yaml_config::MergeSchemas<components::LoggableComponentBase>(R"(
type: object
description: Main service component
additionalProperties: false
properties:
 someUrl:
    type: string
    description: Path to some page
    defaultDescription: ''
)");
}

Service::Service(
		const components::ComponentConfig& conf,
		const components::ComponentContext& ctx)
	: components::LoggableComponentBase{conf, ctx}
	, _repo{ctx.FindComponent<Repository>()}
	, _someUrl{""}
{

	srand((unsigned)time(0));

	// меня пригласили
	_invitations.push_back({rand(), "22222222-2222-2222-2222-222222222222", "8ad16a1d-18b1-4aaa-8b0f-f61915974c66", "user", "1", std::chrono::system_clock::now()});
	_invitations.push_back({rand(), "22222222-2222-2222-2222-222222222222","8ad16a1d-18b1-4aaa-8b0f-f61915974c66", "user", "1", std::chrono::system_clock::now()});
	// Я прошусь
	_invitations.push_back({rand(), "33333333-3333-3333-3333-333333333333", "8ad16a1d-18b1-4aaa-8b0f-f61915974c66", "", "8ad16a1d-18b1-4aaa-8b0f-f61915974c66", std::chrono::system_clock::now()});
	_invitations.push_back({rand(), "44444444-4444-4444-4444-444444444444", "8ad16a1d-18b1-4aaa-8b0f-f61915974c66", "", "8ad16a1d-18b1-4aaa-8b0f-f61915974c66", std::chrono::system_clock::now()});
	// Мы пригласили
	_invitations.push_back({rand(), "55555555-5555-5555-5555-555555555555", "3", "user", "4", std::chrono::system_clock::now()});
	_invitations.push_back({rand(), "66666666-6666-6666-6666-666666666666", "1", "guest", "4", std::chrono::system_clock::now()});
	// Хочет к нам
	_invitations.push_back({rand(), "77777777-7777-7777-7777-777777777777", "3", "user", "3", std::chrono::system_clock::now()});
	_invitations.push_back({rand(), "88888888-8888-8888-8888-888888888888", "15", "guest", "15", std::chrono::system_clock::now()});
	_invitations.push_back({rand(), "99999999-9999-9999-9999-999999999999", "3", "user", "3", std::chrono::system_clock::now()});
	_invitations.push_back({rand(), "11111111-1111-1111-1111-111111111111", "15", "guest", "15", std::chrono::system_clock::now()});
	_invitations.push_back({rand(), "22222222-2222-2222-2222-222222222222", "3", "user", "3", std::chrono::system_clock::now()});
	_invitations.push_back({rand(), "33333333-3333-3333-3333-333333333333", "15", "guest", "15", std::chrono::system_clock::now()});
	_invitations.push_back({rand(), "44444444-4444-4444-4444-444444444444", "3", "user", "3", std::chrono::system_clock::now()});
	_invitations.push_back({rand(), "55555555-5555-5555-5555-555555555555", "15", "guest", "15", std::chrono::system_clock::now()});
	_invitations.push_back({rand(), "66666666-6666-6666-6666-666666666666", "3", "user", "3", std::chrono::system_clock::now()});
	_invitations.push_back({rand(), "77777777-7777-7777-7777-777777777777", "15", "guest", "15", std::chrono::system_clock::now()});
	// Далее данные для Space Detail Page
	// Мы пригласили
	_invitations.push_back({rand(), "11111111-1111-1111-1111-111111111111", "3", "user", "4", std::chrono::system_clock::now()});
	_invitations.push_back({rand(), "11111111-1111-1111-1111-111111111111", "1", "guest", "4", std::chrono::system_clock::now()});
	// Хочет к нам
	_invitations.push_back({rand(), "11111111-1111-1111-1111-111111111111", "3", "user", "3", std::chrono::system_clock::now()});
	_invitations.push_back({rand(), "11111111-1111-1111-1111-111111111111", "15", "guest", "15", std::chrono::system_clock::now()});
	_invitations.push_back({rand(), "11111111-1111-1111-1111-111111111111", "3", "user", "3", std::chrono::system_clock::now()});
	_invitations.push_back({rand(), "11111111-1111-1111-1111-111111111111", "15", "guest", "15", std::chrono::system_clock::now()});
	_invitations.push_back({rand(), "11111111-1111-1111-1111-111111111111", "3", "user", "3", std::chrono::system_clock::now()});
	_invitations.push_back({rand(), "11111111-1111-1111-1111-111111111111", "15", "guest", "15", std::chrono::system_clock::now()});
	_invitations.push_back({rand(), "11111111-1111-1111-1111-111111111111", "3", "user", "3", std::chrono::system_clock::now()});
	_invitations.push_back({rand(), "11111111-1111-1111-1111-111111111111", "15", "guest", "15", std::chrono::system_clock::now()});
	_invitations.push_back({rand(), "11111111-1111-1111-1111-111111111111", "3", "user", "3", std::chrono::system_clock::now()});
	_invitations.push_back({rand(), "11111111-1111-1111-1111-111111111111", "15", "guest", "15", std::chrono::system_clock::now()});

	_links.push_back({boost::uuids::to_string(boost::uuids::random_generator()()), "11111111-1111-1111-1111-111111111111", "8ad16a1d-18b1-4aaa-8b0f-f61915974c66", "link1", std::chrono::system_clock::now(), std::chrono::system_clock::now()});
	_links.push_back({boost::uuids::to_string(boost::uuids::random_generator()()), "22222222-2222-2222-2222-222222222222", "8ad16a1d-18b1-4aaa-8b0f-f61915974c66", "link2", std::chrono::system_clock::now(), std::chrono::system_clock::now()});
	_links.push_back({boost::uuids::to_string(boost::uuids::random_generator()()), "33333333-3333-3333-3333-333333333333", "8ad16a1d-18b1-4aaa-8b0f-f61915974c66", "link3", std::chrono::system_clock::now(), std::chrono::system_clock::now()});
	_links.push_back({boost::uuids::to_string(boost::uuids::random_generator()()), "44444444-4444-4444-4444-444444444444", "8ad16a1d-18b1-4aaa-8b0f-f61915974c66", "link4", std::chrono::system_clock::now(), std::chrono::system_clock::now()});
	_links.push_back({boost::uuids::to_string(boost::uuids::random_generator()()), "55555555-5555-5555-5555-555555555555", "8ad16a1d-18b1-4aaa-8b0f-f61915974c66", "link5", std::chrono::system_clock::now(), std::chrono::system_clock::now()});
	_links.push_back({boost::uuids::to_string(boost::uuids::random_generator()()), "66666666-6666-6666-6666-666666666666", "8ad16a1d-18b1-4aaa-8b0f-f61915974c66", "link6", std::chrono::system_clock::now(), std::chrono::system_clock::now()});
	_links.push_back({boost::uuids::to_string(boost::uuids::random_generator()()), "77777777-7777-7777-7777-777777777777", "8ad16a1d-18b1-4aaa-8b0f-f61915974c66", "link7", std::chrono::system_clock::now(), std::chrono::system_clock::now()});
	_links.push_back({boost::uuids::to_string(boost::uuids::random_generator()()), "88888888-8888-8888-8888-888888888888", "8ad16a1d-18b1-4aaa-8b0f-f61915974c66", "link8", std::chrono::system_clock::now(), std::chrono::system_clock::now()});
	_links.push_back({boost::uuids::to_string(boost::uuids::random_generator()()), "11111111-1111-1111-1111-111111111111", "8ad16a1d-18b1-4aaa-8b0f-f61915974c66", "link11", std::chrono::system_clock::now(), std::chrono::system_clock::now()});
	_links.push_back({boost::uuids::to_string(boost::uuids::random_generator()()), "11111111-1111-1111-1111-111111111111", "8ad16a1d-18b1-4aaa-8b0f-f61915974c66", "link2", std::chrono::system_clock::now(), std::chrono::system_clock::now()});
	_links.push_back({boost::uuids::to_string(boost::uuids::random_generator()()), "11111111-1111-1111-1111-111111111111", "8ad16a1d-18b1-4aaa-8b0f-f61915974c66", "link3", std::chrono::system_clock::now(), std::chrono::system_clock::now()});
	_links.push_back({boost::uuids::to_string(boost::uuids::random_generator()()), "11111111-1111-1111-1111-111111111111", "8ad16a1d-18b1-4aaa-8b0f-f61915974c66", "link4", std::chrono::system_clock::now(), std::chrono::system_clock::now()});
	_links.push_back({boost::uuids::to_string(boost::uuids::random_generator()()), "11111111-1111-1111-1111-111111111111", "8ad16a1d-18b1-4aaa-8b0f-f61915974c66", "link5", std::chrono::system_clock::now(), std::chrono::system_clock::now()});
	_links.push_back({boost::uuids::to_string(boost::uuids::random_generator()()), "11111111-1111-1111-1111-111111111111", "8ad16a1d-18b1-4aaa-8b0f-f61915974c66", "link6", std::chrono::system_clock::now(), std::chrono::system_clock::now()});
	_links.push_back({boost::uuids::to_string(boost::uuids::random_generator()()), "11111111-1111-1111-1111-111111111111", "8ad16a1d-18b1-4aaa-8b0f-f61915974c66", "link7", std::chrono::system_clock::now(), std::chrono::system_clock::now()});
	_links.push_back({boost::uuids::to_string(boost::uuids::random_generator()()), "11111111-1111-1111-1111-111111111111", "8ad16a1d-18b1-4aaa-8b0f-f61915974c66", "link8", std::chrono::system_clock::now(), std::chrono::system_clock::now()});

	// users: SpaceUser[] = [
	// 	{spaceId: "11111111-1111-1111-1111-111111111111", userId: "1", isOwner: false, joinedAt: new Date("2024-10-05"), role: "admin"},
	// 	{spaceId: "11111111-1111-1111-1111-111111111111", userId: "8ad16a1d-18b1-4aaa-8b0f-f61915974c66", isOwner: true, joinedAt: new Date("2024-10-05"), role: "user"},
	// 	{spaceId: "11111111-1111-1111-1111-111111111111", userId: "3", isOwner: false, joinedAt: new Date("2024-10-05"), role: "guest"},
	// 	{spaceId: "11111111-1111-1111-1111-111111111111", userId: "4", isOwner: false, joinedAt: new Date("2024-10-05"), role: "admin"},
	// 	{spaceId: "11111111-1111-1111-1111-111111111111", userId: "5", isOwner: false, joinedAt: new Date("2024-10-05"), role: "user"},
	// 	{spaceId: "11111111-1111-1111-1111-111111111111", userId: "6", isOwner: false, joinedAt: new Date("2024-10-05"), role: "guest"},
	// 	{spaceId: "11111111-1111-1111-1111-111111111111", userId: "7", isOwner: false, joinedAt: new Date("2024-10-05"), role: "admin"},
	// 	{spaceId: "11111111-1111-1111-1111-111111111111", userId: "8", isOwner: false, joinedAt: new Date("2024-10-05"), role: "user"},
	// 	{spaceId: "11111111-1111-1111-1111-111111111111", userId: "9", isOwner: false, joinedAt: new Date("2024-10-05"), role: "guest"},
	// 	{spaceId: "11111111-1111-1111-1111-111111111111", userId: "10", isOwner: false, joinedAt: new Date("2024-10-05"), role: "admin"},
	// 	{spaceId: "11111111-1111-1111-1111-111111111111", userId: "11", isOwner: false, joinedAt: new Date("2024-10-05"), role: "user"},
	// 	{spaceId: "11111111-1111-1111-1111-111111111111", userId: "12", isOwner: false, joinedAt: new Date("2024-10-05"), role: "guest"},
	// 	{spaceId: "11111111-1111-1111-1111-111111111111", userId: "13", isOwner: false, joinedAt: new Date("2024-10-05"), role: "admin"},
	// 	{spaceId: "11111111-1111-1111-1111-111111111111", userId: "14", isOwner: false, joinedAt: new Date("2024-10-05"), role: "user"},
	// 	{spaceId: "11111111-1111-1111-1111-111111111111", userId: "15", isOwner: false, joinedAt: new Date("2024-10-05"), role: "guest"},
	// ];

}

std::vector<svetit::space::model::Space> Service::GetList(unsigned int start, unsigned int limit)
{
	return _repo.Space().Select(start,limit);
}

int Service::GetSize() {
	return _repo.Space().Count();
}

std::vector<svetit::space::model::SpaceInvitation> Service::GetInvitationList(unsigned int start, unsigned int limit)
{
	std::vector<svetit::space::model::SpaceInvitation>::const_iterator first = _invitations.begin() + start;
	std::vector<svetit::space::model::SpaceInvitation>::const_iterator last;

	if (start + limit > _invitations.size()) {
		last = _invitations.end();
	} else {
		last = _invitations.begin() + start + limit;
	}

	return std::vector<svetit::space::model::SpaceInvitation>(first, last);
}

int Service::GetInvitationsSize() {
	return _invitations.size();
}

std::vector<svetit::space::model::SpaceLink> Service::GetLinkList(unsigned int start, unsigned int limit)
{
	std::vector<svetit::space::model::SpaceLink>::const_iterator first = _links.begin() + start;
	std::vector<svetit::space::model::SpaceLink>::const_iterator last;

	if (start + limit > _links.size()) {
		last = _links.end();
	} else {
		last = _links.begin() + start + limit;
	}

	return std::vector<svetit::space::model::SpaceLink>(first, last);
}

int Service::GetLinksSize() {
	return _links.size();
}

std::vector<svetit::space::model::SpaceUser> Service::GetUserList()
{
	return _users;
}

} // namespace svetit::space