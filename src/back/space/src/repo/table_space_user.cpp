#include "table_space_user.hpp"

#include <userver/components/component_config.hpp>
#include <userver/components/component_context.hpp>
#include <userver/utils/boost_uuid4.hpp>
#include <userver/yaml_config/merge_schemas.hpp>
#include <userver/storages/postgres/component.hpp>

namespace svetit::space::table {

namespace pg = storages::postgres;

SpaceUser::SpaceUser(storages::postgres::ClusterPtr pg)
	: _pg{std::move(pg)}
{
	constexpr auto kCreateTable = R"~(
CREATE TABLE IF NOT EXISTS space_user (
	spaceId uuid NOT NULL,
	userId uuid NOT NULL,
	isOwner BOOLEAN NOT NULL,
	joinedAt TIMESTAMPTZ NOT NULL DEFAULT NOW(),
	role TEXT NOT NULL,
	requestsAllowed BOOLEAN NOT NULL
);
)~";

	using storages::postgres::ClusterHostType;
	_pg->Execute(ClusterHostType::kMaster, kCreateTable);

	//InsertDataForMocks();
}

const storages::postgres::Query kInsertSpaceUser{
	"INSERT INTO space_user (spaceId, userId, isOwner, joinedAt, role, requestsAllowed) "
	"VALUES ($1, $2, $3, $4, $5, $6) ",
	storages::postgres::Query::Name{"insert_space_user"},
};

void SpaceUser::Insert(
	const boost::uuids::uuid& spaceId,
	const boost::uuids::uuid& userId,
	const bool& isOwner,
	std::chrono::system_clock::time_point joinedAt,
	const std::string& role,
	const bool requestsAllowed
	)
{
	storages::postgres::Transaction transaction =
		_pg->Begin("insert_space_user_transaction",
			storages::postgres::ClusterHostType::kMaster, {});

	transaction.Execute(kInsertSpaceUser, spaceId, userId, isOwner, joinedAt, role, requestsAllowed);
	transaction.Commit();
}

void SpaceUser::InsertDataForMocks() {
	Insert(utils::BoostUuidFromString("11111111-1111-1111-1111-111111111111"), utils::BoostUuidFromString("8ad16a1d-18b1-4aaa-8b0f-f61915974c66"), false, std::chrono::system_clock::now(), "admin", true);
	Insert(utils::BoostUuidFromString("11111111-1111-1111-1111-111111111111"), utils::BoostUuidFromString("02d16a1d-18b1-4aaa-8b0f-f61915974c66"), true, std::chrono::system_clock::now(), "user", true);
	Insert(utils::BoostUuidFromString("11111111-1111-1111-1111-111111111111"), utils::BoostUuidFromString("03d16a1d-18b1-4aaa-8b0f-f61915974c66"), false, std::chrono::system_clock::now(), "guest", true);
	Insert(utils::BoostUuidFromString("11111111-1111-1111-1111-111111111111"), utils::BoostUuidFromString("04d16a1d-18b1-4aaa-8b0f-f61915974c66"), false, std::chrono::system_clock::now(), "admin", true);
	Insert(utils::BoostUuidFromString("11111111-1111-1111-1111-111111111111"), utils::BoostUuidFromString("05d16a1d-18b1-4aaa-8b0f-f61915974c66"), false, std::chrono::system_clock::now(), "user", true);
	Insert(utils::BoostUuidFromString("11111111-1111-1111-1111-111111111111"), utils::BoostUuidFromString("06d16a1d-18b1-4aaa-8b0f-f61915974c66"), false, std::chrono::system_clock::now(), "guest", true);
	Insert(utils::BoostUuidFromString("11111111-1111-1111-1111-111111111111"), utils::BoostUuidFromString("07d16a1d-18b1-4aaa-8b0f-f61915974c66"), false, std::chrono::system_clock::now(), "admin", true);
	Insert(utils::BoostUuidFromString("11111111-1111-1111-1111-111111111111"), utils::BoostUuidFromString("08d16a1d-18b1-4aaa-8b0f-f61915974c66"), false, std::chrono::system_clock::now(), "user", true);
	Insert(utils::BoostUuidFromString("11111111-1111-1111-1111-111111111111"), utils::BoostUuidFromString("09d16a1d-18b1-4aaa-8b0f-f61915974c66"), false, std::chrono::system_clock::now(), "guest", true);
	Insert(utils::BoostUuidFromString("11111111-1111-1111-1111-111111111111"), utils::BoostUuidFromString("10d16a1d-18b1-4aaa-8b0f-f61915974c66"), false, std::chrono::system_clock::now(), "admin", true);
	Insert(utils::BoostUuidFromString("11111111-1111-1111-1111-111111111111"), utils::BoostUuidFromString("11d16a1d-18b1-4aaa-8b0f-f61915974c66"), false, std::chrono::system_clock::now(), "user", true);
	Insert(utils::BoostUuidFromString("11111111-1111-1111-1111-111111111111"), utils::BoostUuidFromString("12d16a1d-18b1-4aaa-8b0f-f61915974c66"), false, std::chrono::system_clock::now(), "guest", true);
	Insert(utils::BoostUuidFromString("11111111-1111-1111-1111-111111111111"), utils::BoostUuidFromString("13d16a1d-18b1-4aaa-8b0f-f61915974c66"), false, std::chrono::system_clock::now(), "admin", true);
	Insert(utils::BoostUuidFromString("11111111-1111-1111-1111-111111111111"), utils::BoostUuidFromString("14d16a1d-18b1-4aaa-8b0f-f61915974c66"), false, std::chrono::system_clock::now(), "user", true);
	Insert(utils::BoostUuidFromString("11111111-1111-1111-1111-111111111111"), utils::BoostUuidFromString("15d16a1d-18b1-4aaa-8b0f-f61915974c66"), false, std::chrono::system_clock::now(), "guest", true);
}

} // namespace svetit::space::table
