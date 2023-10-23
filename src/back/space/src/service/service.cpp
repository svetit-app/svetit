#include "service.hpp"
#include "../model/model.hpp"

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
	, _someUrl{""}
{
	_spaces.push_back({"11111111-1111-1111-1111-111111111111", "Пространство №1", "key1", true, std::chrono::system_clock::now()});
	_spaces.push_back({"22222222-2222-2222-2222-222222222222", "Пространство №2", "key2", true, std::chrono::system_clock::now()});
	_spaces.push_back({"33333333-3333-3333-3333-333333333333", "Пространство №3", "key3", true, std::chrono::system_clock::now()});
	_spaces.push_back({"44444444-4444-4444-4444-444444444444", "Пространство №4", "key4", true, std::chrono::system_clock::now()});
	_spaces.push_back({"55555555-5555-5555-5555-555555555555", "Пространство №5", "key5", true, std::chrono::system_clock::now()});
	_spaces.push_back({"66666666-6666-6666-6666-666666666666", "Пространство №6", "key6", true, std::chrono::system_clock::now()});
	_spaces.push_back({"77777777-7777-7777-7777-777777777777", "Пространство №7", "key7", true, std::chrono::system_clock::now()});
	_spaces.push_back({"88888888-8888-8888-8888-888888888888", "Пространство №8", "key8", true, std::chrono::system_clock::now()});
	_spaces.push_back({"99999999-9999-9999-9999-999999999999", "Пространство №9", "key9", true, std::chrono::system_clock::now()});
	_spaces.push_back({"10000000-1000-1000-1000-100000000000", "Пространство №10", "key10", true, std::chrono::system_clock::now()});
	_spaces.push_back({"11000000-1100-1100-1100-110000000000", "Пространство №11", "key11", true, std::chrono::system_clock::now()});

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

	// links: SpaceLink[] = [
	// 	{id: crypto.randomUUID(), spaceId: '11111111-1111-1111-1111-111111111111', creatorId: "8ad16a1d-18b1-4aaa-8b0f-f61915974c66", name: "link1", createdAt: new Date("2023-10-08"), expiredAt: new Date("2024-10-08")},
	// 	{id: crypto.randomUUID(), spaceId: '22222222-2222-2222-2222-222222222222', creatorId: "8ad16a1d-18b1-4aaa-8b0f-f61915974c66", name: "link2", createdAt: new Date("2023-10-08"), expiredAt: new Date("2024-10-08")},
	// 	{id: crypto.randomUUID(), spaceId: '33333333-3333-3333-3333-333333333333', creatorId: "8ad16a1d-18b1-4aaa-8b0f-f61915974c66", name: "link3", createdAt: new Date("2023-10-08"), expiredAt: new Date("2024-10-08")},
	// 	{id: crypto.randomUUID(), spaceId: '44444444-4444-4444-4444-444444444444', creatorId: "8ad16a1d-18b1-4aaa-8b0f-f61915974c66", name: "link4", createdAt: new Date("2023-10-08"), expiredAt: new Date("2024-10-08")},
	// 	{id: crypto.randomUUID(), spaceId: '55555555-5555-5555-5555-555555555555', creatorId: "8ad16a1d-18b1-4aaa-8b0f-f61915974c66", name: "link5", createdAt: new Date("2023-10-08"), expiredAt: new Date("2024-10-08")},
	// 	{id: crypto.randomUUID(), spaceId: '66666666-6666-6666-6666-666666666666', creatorId: "8ad16a1d-18b1-4aaa-8b0f-f61915974c66", name: "link6", createdAt: new Date("2023-10-08"), expiredAt: new Date("2024-10-08")},
	// 	{id: crypto.randomUUID(), spaceId: '77777777-7777-7777-7777-777777777777', creatorId: "8ad16a1d-18b1-4aaa-8b0f-f61915974c66", name: "link7", createdAt: new Date("2023-10-08"), expiredAt: new Date("2024-10-08")},
	// 	{id: crypto.randomUUID(), spaceId: '88888888-8888-8888-8888-888888888888', creatorId: "8ad16a1d-18b1-4aaa-8b0f-f61915974c66", name: "link8", createdAt: new Date("2023-10-08"), expiredAt: new Date("2024-10-08")},
	// 	{id: crypto.randomUUID(), spaceId: '11111111-1111-1111-1111-111111111111', creatorId: "8ad16a1d-18b1-4aaa-8b0f-f61915974c66", name: "link11", createdAt: new Date("2023-10-08"), expiredAt: new Date("2024-10-08")},
	// 	{id: crypto.randomUUID(), spaceId: '11111111-1111-1111-1111-111111111111', creatorId: "8ad16a1d-18b1-4aaa-8b0f-f61915974c66", name: "link2", createdAt: new Date("2023-10-08"), expiredAt: new Date("2024-10-08")},
	// 	{id: crypto.randomUUID(), spaceId: '11111111-1111-1111-1111-111111111111', creatorId: "8ad16a1d-18b1-4aaa-8b0f-f61915974c66", name: "link3", createdAt: new Date("2023-10-08"), expiredAt: new Date("2024-10-08")},
	// 	{id: crypto.randomUUID(), spaceId: '11111111-1111-1111-1111-111111111111', creatorId: "8ad16a1d-18b1-4aaa-8b0f-f61915974c66", name: "link4", createdAt: new Date("2023-10-08"), expiredAt: new Date("2024-10-08")},
	// 	{id: crypto.randomUUID(), spaceId: '11111111-1111-1111-1111-111111111111', creatorId: "8ad16a1d-18b1-4aaa-8b0f-f61915974c66", name: "link5", createdAt: new Date("2023-10-08"), expiredAt: new Date("2024-10-08")},
	// 	{id: crypto.randomUUID(), spaceId: '11111111-1111-1111-1111-111111111111', creatorId: "8ad16a1d-18b1-4aaa-8b0f-f61915974c66", name: "link6", createdAt: new Date("2023-10-08"), expiredAt: new Date("2024-10-08")},
	// 	{id: crypto.randomUUID(), spaceId: '11111111-1111-1111-1111-111111111111', creatorId: "8ad16a1d-18b1-4aaa-8b0f-f61915974c66", name: "link7", createdAt: new Date("2023-10-08"), expiredAt: new Date("2024-10-08")},
	// 	{id: crypto.randomUUID(), spaceId: '11111111-1111-1111-1111-111111111111', creatorId: "8ad16a1d-18b1-4aaa-8b0f-f61915974c66", name: "link8", createdAt: new Date("2023-10-08"), expiredAt: new Date("2024-10-08")},
	// ];

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

std::vector<svetit::space::Space> Service::GetList()
{
	return _spaces;
}

std::vector<svetit::space::SpaceInvitation> Service::GetInvitationList()
{
	return _invitations;
}

std::vector<svetit::space::SpaceLink> Service::GetLinkList()
{
	return _links;
}

std::vector<svetit::space::SpaceUser> Service::GetUserList()
{
	return _users;
}

} // namespace svetit::space