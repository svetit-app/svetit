#include "table_space_invitation.hpp"

#include <userver/components/component_config.hpp>
#include <userver/components/component_context.hpp>
#include <userver/utils/boost_uuid4.hpp>
#include <userver/yaml_config/merge_schemas.hpp>
#include <userver/storages/postgres/component.hpp>

namespace svetit::space::table {

namespace pg = storages::postgres;

SpaceInvitation::SpaceInvitation(storages::postgres::ClusterPtr pg)
	: _pg{std::move(pg)}
{
	constexpr auto kCreateTable = R"~(
CREATE TABLE IF NOT EXISTS space_invitation (
	id serial PRIMARY KEY,
	spaceId uuid,
	creatorId uuid,
	userId uuid,
	role TEXT NOT NULL,
	createdAt TIMESTAMPTZ NOT NULL DEFAULT NOW()
);
)~";

	using storages::postgres::ClusterHostType;
	_pg->Execute(ClusterHostType::kMaster, kCreateTable);

	//InsertDataForMocks();
}

const storages::postgres::Query kInsertSpaceInvitation{
	"INSERT INTO space_invitation (spaceId, userId, role, creatorId, createdAt) "
	"VALUES ($1, $2, $3, $4, $5) ",
	storages::postgres::Query::Name{"insert_space_invitation"},
};

void SpaceInvitation::Insert(
	const boost::uuids::uuid& spaceId,
	const boost::uuids::uuid& userId,
	const std::string& role,
	const boost::uuids::uuid& creatorId,
	std::chrono::system_clock::time_point createdAt)
{
	storages::postgres::Transaction transaction =
		_pg->Begin("insert_space_invitation_transaction",
			storages::postgres::ClusterHostType::kMaster, {});

	transaction.Execute(kInsertSpaceInvitation, spaceId, userId, role, creatorId, createdAt);
	transaction.Commit();
}

const storages::postgres::Query kSelectSpaceInvitation{
	"SELECT id, spaceId, creatorId, userId, role, createdAt FROM space_invitation OFFSET $1 LIMIT $2",
	storages::postgres::Query::Name{"select_space_invitation"},
};

std::vector<model::SpaceInvitation> SpaceInvitation::Select(const int& offset, const int& limit)
{
	storages::postgres::Transaction transaction =
		_pg->Begin("select_space_invitation_transaction",
			storages::postgres::ClusterHostType::kMaster, {});

	auto res = transaction.Execute(kSelectSpaceInvitation, offset, limit);
	if (res.IsEmpty())
	{
		transaction.Commit();
		return {};
	}

	transaction.Commit();
	return res.AsContainer<std::vector<model::SpaceInvitation>>(pg::kRowTag);
}

const storages::postgres::Query kCountSpaceInvitation{
	"SELECT count(id) FROM space_invitation",
	storages::postgres::Query::Name{"count_space_invitation"},
};

int SpaceInvitation::Count() {
	storages::postgres::Transaction transaction =
		_pg->Begin("count_space_invitation_transaction",
			storages::postgres::ClusterHostType::kMaster, {});

	auto res = transaction.Execute(kCountSpaceInvitation);

	auto id = res.Front()[0].As<int64_t>();
	transaction.Commit();

	return id;
}

const storages::postgres::Query kCountInvitationsAvailable{
	"SELECT (SELECT count(id) FROM space_invitation WHERE creatorId != userId AND userId = $1) + (SELECT count(id) FROM space_invitation WHERE creatorId = userId AND userId != $1) AS SumCount" ,
	storages::postgres::Query::Name{"count_space_invitation_available"},
};

int SpaceInvitation::GetAvailableCount(boost::uuids::uuid currentUserId) {

	storages::postgres::Transaction transaction =
		_pg->Begin("count_space_invitation_available_transaction",
			storages::postgres::ClusterHostType::kMaster, {});

	const auto res = transaction.Execute(kCountInvitationsAvailable, currentUserId);

	const auto count = res.Front()[0].As<int64_t>();

	transaction.Commit();

	return count;
}

const storages::postgres::Query kDeleteBySpace {
	"DELETE FROM space_invitation WHERE spaceId = $1",
	storages::postgres::Query::Name{"delete_space_invitation_by_space"},
};

void SpaceInvitation::DeleteBySpace(boost::uuids::uuid spaceUuid) {
	storages::postgres::Transaction transaction =
		_pg->Begin("delete_space_invitation_by_space_transaction",
			storages::postgres::ClusterHostType::kMaster, {});

	auto res = transaction.Execute(kDeleteBySpace, spaceUuid);

	transaction.Commit();
}

void SpaceInvitation::InsertDataForMocks() {
	// insert test data
	// меня пригласили
	Insert(utils::BoostUuidFromString("22222222-2222-2222-2222-222222222222"), utils::BoostUuidFromString("8ad16a1d-18b1-4aaa-8b0f-f61915974c66"), "user", utils::BoostUuidFromString("01000000-0000-0000-0000-000000000000"), std::chrono::system_clock::now());
	Insert(utils::BoostUuidFromString("22222222-2222-2222-2222-222222222222"),utils::BoostUuidFromString("8ad16a1d-18b1-4aaa-8b0f-f61915974c66"), "user", utils::BoostUuidFromString("01000000-0000-0000-0000-000000000000"), std::chrono::system_clock::now());
	// Я прошусь
	Insert(utils::BoostUuidFromString("33333333-3333-3333-3333-333333333333"), utils::BoostUuidFromString("8ad16a1d-18b1-4aaa-8b0f-f61915974c66"), "", utils::BoostUuidFromString("8ad16a1d-18b1-4aaa-8b0f-f61915974c66"), std::chrono::system_clock::now());
	Insert(utils::BoostUuidFromString("44444444-4444-4444-4444-444444444444"), utils::BoostUuidFromString("8ad16a1d-18b1-4aaa-8b0f-f61915974c66"), "", utils::BoostUuidFromString("8ad16a1d-18b1-4aaa-8b0f-f61915974c66"), std::chrono::system_clock::now());
	// Мы пригласили
	Insert(utils::BoostUuidFromString("55555555-5555-5555-5555-555555555555"), utils::BoostUuidFromString("03000000-0000-0000-0000-000000000000"), "user", utils::BoostUuidFromString("04000000-0000-0000-0000-000000000000"), std::chrono::system_clock::now());
	Insert(utils::BoostUuidFromString("66666666-6666-6666-6666-666666666666"), utils::BoostUuidFromString("01000000-0000-0000-0000-000000000000"), "guest", utils::BoostUuidFromString("04000000-0000-0000-0000-000000000000"), std::chrono::system_clock::now());
	// Хочет к нам
	Insert(utils::BoostUuidFromString("77777777-7777-7777-7777-777777777777"), utils::BoostUuidFromString("03000000-0000-0000-0000-000000000000"), "user", utils::BoostUuidFromString("03000000-0000-0000-0000-000000000000"), std::chrono::system_clock::now());
	Insert(utils::BoostUuidFromString("88888888-8888-8888-8888-888888888888"), utils::BoostUuidFromString("15000000-0000-0000-0000-000000000000"), "guest", utils::BoostUuidFromString("15000000-0000-0000-0000-000000000000"), std::chrono::system_clock::now());
	Insert(utils::BoostUuidFromString("99999999-9999-9999-9999-999999999999"), utils::BoostUuidFromString("03000000-0000-0000-0000-000000000000"), "user", utils::BoostUuidFromString("03000000-0000-0000-0000-000000000000"), std::chrono::system_clock::now());
	Insert(utils::BoostUuidFromString("11111111-1111-1111-1111-111111111111"), utils::BoostUuidFromString("15000000-0000-0000-0000-000000000000"), "guest", utils::BoostUuidFromString("15000000-0000-0000-0000-000000000000"), std::chrono::system_clock::now());
	Insert(utils::BoostUuidFromString("22222222-2222-2222-2222-222222222222"), utils::BoostUuidFromString("03000000-0000-0000-0000-000000000000"), "user", utils::BoostUuidFromString("03000000-0000-0000-0000-000000000000"), std::chrono::system_clock::now());
	Insert(utils::BoostUuidFromString("33333333-3333-3333-3333-333333333333"), utils::BoostUuidFromString("15000000-0000-0000-0000-000000000000"), "guest", utils::BoostUuidFromString("15000000-0000-0000-0000-000000000000"), std::chrono::system_clock::now());
	Insert(utils::BoostUuidFromString("44444444-4444-4444-4444-444444444444"), utils::BoostUuidFromString("03000000-0000-0000-0000-000000000000"), "user", utils::BoostUuidFromString("03000000-0000-0000-0000-000000000000"), std::chrono::system_clock::now());
	Insert(utils::BoostUuidFromString("55555555-5555-5555-5555-555555555555"), utils::BoostUuidFromString("15000000-0000-0000-0000-000000000000"), "guest", utils::BoostUuidFromString("15000000-0000-0000-0000-000000000000"), std::chrono::system_clock::now());
	Insert(utils::BoostUuidFromString("66666666-6666-6666-6666-666666666666"), utils::BoostUuidFromString("03000000-0000-0000-0000-000000000000"), "user", utils::BoostUuidFromString("03000000-0000-0000-0000-000000000000"), std::chrono::system_clock::now());
	Insert(utils::BoostUuidFromString("77777777-7777-7777-7777-777777777777"), utils::BoostUuidFromString("15000000-0000-0000-0000-000000000000"), "guest", utils::BoostUuidFromString("15000000-0000-0000-0000-000000000000"), std::chrono::system_clock::now());
	// Далее данные для Space Detail Page
	// Мы пригласили
	Insert(utils::BoostUuidFromString("11111111-1111-1111-1111-111111111111"), utils::BoostUuidFromString("03000000-0000-0000-0000-000000000000"), "user", utils::BoostUuidFromString("04000000-0000-0000-0000-000000000000"), std::chrono::system_clock::now());
	Insert(utils::BoostUuidFromString("11111111-1111-1111-1111-111111111111"), utils::BoostUuidFromString("01000000-0000-0000-0000-000000000000"), "guest", utils::BoostUuidFromString("04000000-0000-0000-0000-000000000000"), std::chrono::system_clock::now());
	// Хочет к нам
	Insert(utils::BoostUuidFromString("11111111-1111-1111-1111-111111111111"), utils::BoostUuidFromString("03000000-0000-0000-0000-000000000000"), "user", utils::BoostUuidFromString("03000000-0000-0000-0000-000000000000"), std::chrono::system_clock::now());
	Insert(utils::BoostUuidFromString("11111111-1111-1111-1111-111111111111"), utils::BoostUuidFromString("15000000-0000-0000-0000-000000000000"), "guest", utils::BoostUuidFromString("15000000-0000-0000-0000-000000000000"), std::chrono::system_clock::now());
	Insert(utils::BoostUuidFromString("11111111-1111-1111-1111-111111111111"), utils::BoostUuidFromString("03000000-0000-0000-0000-000000000000"), "user", utils::BoostUuidFromString("03000000-0000-0000-0000-000000000000"), std::chrono::system_clock::now());
	Insert(utils::BoostUuidFromString("11111111-1111-1111-1111-111111111111"), utils::BoostUuidFromString("15000000-0000-0000-0000-000000000000"), "guest", utils::BoostUuidFromString("15000000-0000-0000-0000-000000000000"), std::chrono::system_clock::now());
	Insert(utils::BoostUuidFromString("11111111-1111-1111-1111-111111111111"), utils::BoostUuidFromString("03000000-0000-0000-0000-000000000000"), "user", utils::BoostUuidFromString("03000000-0000-0000-0000-000000000000"), std::chrono::system_clock::now());
	Insert(utils::BoostUuidFromString("11111111-1111-1111-1111-111111111111"), utils::BoostUuidFromString("15000000-0000-0000-0000-000000000000"), "guest", utils::BoostUuidFromString("15000000-0000-0000-0000-000000000000"), std::chrono::system_clock::now());
	Insert(utils::BoostUuidFromString("11111111-1111-1111-1111-111111111111"), utils::BoostUuidFromString("03000000-0000-0000-0000-000000000000"), "user", utils::BoostUuidFromString("03000000-0000-0000-0000-000000000000"), std::chrono::system_clock::now());
	Insert(utils::BoostUuidFromString("11111111-1111-1111-1111-111111111111"), utils::BoostUuidFromString("15000000-0000-0000-0000-000000000000"), "guest", utils::BoostUuidFromString("15000000-0000-0000-0000-000000000000"), std::chrono::system_clock::now());
	Insert(utils::BoostUuidFromString("11111111-1111-1111-1111-111111111111"), utils::BoostUuidFromString("03000000-0000-0000-0000-000000000000"), "user", utils::BoostUuidFromString("03000000-0000-0000-0000-000000000000"), std::chrono::system_clock::now());
	Insert(utils::BoostUuidFromString("11111111-1111-1111-1111-111111111111"), utils::BoostUuidFromString("15000000-0000-0000-0000-000000000000"), "guest", utils::BoostUuidFromString("15000000-0000-0000-0000-000000000000"), std::chrono::system_clock::now());
}

} // namespace svetit::space::table