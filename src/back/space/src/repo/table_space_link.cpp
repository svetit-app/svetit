#include "table_space_link.hpp"

#include <userver/components/component_config.hpp>
#include <userver/components/component_context.hpp>
#include <userver/utils/boost_uuid4.hpp>
#include <userver/yaml_config/merge_schemas.hpp>
#include <userver/storages/postgres/component.hpp>
#include <boost/uuid/uuid_generators.hpp>
#include <boost/uuid/uuid_io.hpp>
#include <boost/uuid/uuid.hpp>

namespace svetit::space::table {

namespace pg = storages::postgres;

SpaceLink::SpaceLink(storages::postgres::ClusterPtr pg)
	: _pg{std::move(pg)}
{
	constexpr auto kCreateTable = R"~(
CREATE TABLE IF NOT EXISTS space_link (
	id uuid PRIMARY KEY,
	spaceId uuid,
	creatorId uuid,
	name TEXT NOT NULL,
	createdAt TIMESTAMPTZ NOT NULL DEFAULT NOW(),
	expiredAt TIMESTAMPTZ NOT NULL
);
)~";

	using storages::postgres::ClusterHostType;
	_pg->Execute(ClusterHostType::kMaster, kCreateTable);

	//InsertDataForMocks();
}

const storages::postgres::Query kInsertSpaceLink{
	"INSERT INTO space_link (id, spaceId, creatorId, name, createdAt, expiredAt) "
	"VALUES ($1, $2, $3, $4, $5, $6) ",
	storages::postgres::Query::Name{"insert_space_link"},
};

void SpaceLink::Insert(
	const boost::uuids::uuid& id,
	const boost::uuids::uuid& spaceId,
	const boost::uuids::uuid& creatorId,
	const std::string& name,
	std::chrono::system_clock::time_point createdAt,
	std::chrono::system_clock::time_point expiredAt)
{
	storages::postgres::Transaction transaction =
		_pg->Begin("insert_space_link_transaction",
			storages::postgres::ClusterHostType::kMaster, {});

	transaction.Execute(kInsertSpaceLink, id, spaceId, creatorId, name, createdAt, expiredAt);
	transaction.Commit();
}

const storages::postgres::Query kSelectSpaceLink{
	"SELECT id, spaceId, creatorId, name, createdAt, expiredAt FROM space_link OFFSET $1 LIMIT $2",
	storages::postgres::Query::Name{"select_space_link"},
};

std::vector<model::SpaceLink> SpaceLink::Select(const int& offset, const int& limit)
{
	storages::postgres::Transaction transaction =
		_pg->Begin("select_space_link_transaction",
			storages::postgres::ClusterHostType::kMaster, {});

	auto res = transaction.Execute(kSelectSpaceLink, offset, limit);
	if (res.IsEmpty())
	{
		transaction.Commit();
		return {};
	}

	transaction.Commit();
	// todo - for what pg::kRowTag here?
	return res.AsContainer<std::vector<model::SpaceLink>>(pg::kRowTag);
}

const storages::postgres::Query kCountSpaceLink{
	"SELECT count(id) FROM space_link",
	storages::postgres::Query::Name{"count_space_link"},
};

int SpaceLink::Count() {
	storages::postgres::Transaction transaction =
		_pg->Begin("count_space_link_transaction",
			storages::postgres::ClusterHostType::kMaster, {});

	auto res = transaction.Execute(kCountSpaceLink);

	auto id = res.Front()[0].As<int64_t>();
	transaction.Commit();

	return id;
}

const storages::postgres::Query kDeleteBySpace {
	"DELETE FROM space_link WHERE spaceId = $1",
	storages::postgres::Query::Name{"delete_space_link_by_space"},
};

bool SpaceLink::DeleteBySpace(boost::uuids::uuid spaceUuid) {
	storages::postgres::Transaction transaction =
		_pg->Begin("delete_space_link_by_space_transaction",
			storages::postgres::ClusterHostType::kMaster, {});

	auto res = transaction.Execute(kDeleteBySpace, spaceUuid);

	transaction.Commit();
	return res.RowsAffected();
}

const storages::postgres::Query kDeleteById {
	"DELETE FROM space_link WHERE id = $1",
	storages::postgres::Query::Name{"delete_space_link_by_id"},
};

bool SpaceLink::DeleteById(const boost::uuids::uuid id) {
	storages::postgres::Transaction transaction =
		_pg->Begin("delete_space_link_by_id_transaction",
			storages::postgres::ClusterHostType::kMaster, {});

	auto res = transaction.Execute(kDeleteById, id);
	transaction.Commit();

	return res.RowsAffected();
}

void SpaceLink::InsertDataForMocks() {
	Insert(boost::uuids::random_generator()(), utils::BoostUuidFromString("11111111-1111-1111-1111-111111111111"), utils::BoostUuidFromString("8ad16a1d-18b1-4aaa-8b0f-f61915974c66"), "link1", std::chrono::system_clock::now(), std::chrono::system_clock::now());
	Insert(boost::uuids::random_generator()(), utils::BoostUuidFromString("22222222-2222-2222-2222-222222222222"), utils::BoostUuidFromString("8ad16a1d-18b1-4aaa-8b0f-f61915974c66"), "link2", std::chrono::system_clock::now(), std::chrono::system_clock::now());
	Insert(boost::uuids::random_generator()(), utils::BoostUuidFromString("33333333-3333-3333-3333-333333333333"), utils::BoostUuidFromString("8ad16a1d-18b1-4aaa-8b0f-f61915974c66"), "link3", std::chrono::system_clock::now(), std::chrono::system_clock::now());
	Insert(boost::uuids::random_generator()(), utils::BoostUuidFromString("44444444-4444-4444-4444-444444444444"), utils::BoostUuidFromString("8ad16a1d-18b1-4aaa-8b0f-f61915974c66"), "link4", std::chrono::system_clock::now(), std::chrono::system_clock::now());
	Insert(boost::uuids::random_generator()(), utils::BoostUuidFromString("55555555-5555-5555-5555-555555555555"), utils::BoostUuidFromString("8ad16a1d-18b1-4aaa-8b0f-f61915974c66"), "link5", std::chrono::system_clock::now(), std::chrono::system_clock::now());
	Insert(boost::uuids::random_generator()(), utils::BoostUuidFromString("66666666-6666-6666-6666-666666666666"), utils::BoostUuidFromString("8ad16a1d-18b1-4aaa-8b0f-f61915974c66"), "link6", std::chrono::system_clock::now(), std::chrono::system_clock::now());
	Insert(boost::uuids::random_generator()(), utils::BoostUuidFromString("77777777-7777-7777-7777-777777777777"), utils::BoostUuidFromString("8ad16a1d-18b1-4aaa-8b0f-f61915974c66"), "link7", std::chrono::system_clock::now(), std::chrono::system_clock::now());
	Insert(boost::uuids::random_generator()(), utils::BoostUuidFromString("88888888-8888-8888-8888-888888888888"), utils::BoostUuidFromString("8ad16a1d-18b1-4aaa-8b0f-f61915974c66"), "link8", std::chrono::system_clock::now(), std::chrono::system_clock::now());
	Insert(boost::uuids::random_generator()(), utils::BoostUuidFromString("11111111-1111-1111-1111-111111111111"), utils::BoostUuidFromString("8ad16a1d-18b1-4aaa-8b0f-f61915974c66"), "link11", std::chrono::system_clock::now(), std::chrono::system_clock::now());
	Insert(boost::uuids::random_generator()(), utils::BoostUuidFromString("11111111-1111-1111-1111-111111111111"), utils::BoostUuidFromString("8ad16a1d-18b1-4aaa-8b0f-f61915974c66"), "link2", std::chrono::system_clock::now(), std::chrono::system_clock::now());
	Insert(boost::uuids::random_generator()(), utils::BoostUuidFromString("11111111-1111-1111-1111-111111111111"), utils::BoostUuidFromString("8ad16a1d-18b1-4aaa-8b0f-f61915974c66"), "link3", std::chrono::system_clock::now(), std::chrono::system_clock::now());
	Insert(boost::uuids::random_generator()(), utils::BoostUuidFromString("11111111-1111-1111-1111-111111111111"), utils::BoostUuidFromString("8ad16a1d-18b1-4aaa-8b0f-f61915974c66"), "link4", std::chrono::system_clock::now(), std::chrono::system_clock::now());
	Insert(boost::uuids::random_generator()(), utils::BoostUuidFromString("11111111-1111-1111-1111-111111111111"), utils::BoostUuidFromString("8ad16a1d-18b1-4aaa-8b0f-f61915974c66"), "link5", std::chrono::system_clock::now(), std::chrono::system_clock::now());
	Insert(boost::uuids::random_generator()(), utils::BoostUuidFromString("11111111-1111-1111-1111-111111111111"), utils::BoostUuidFromString("8ad16a1d-18b1-4aaa-8b0f-f61915974c66"), "link6", std::chrono::system_clock::now(), std::chrono::system_clock::now());
	Insert(boost::uuids::random_generator()(), utils::BoostUuidFromString("11111111-1111-1111-1111-111111111111"), utils::BoostUuidFromString("8ad16a1d-18b1-4aaa-8b0f-f61915974c66"), "link7", std::chrono::system_clock::now(), std::chrono::system_clock::now());
	Insert(boost::uuids::random_generator()(), utils::BoostUuidFromString("11111111-1111-1111-1111-111111111111"), utils::BoostUuidFromString("8ad16a1d-18b1-4aaa-8b0f-f61915974c66"), "link8", std::chrono::system_clock::now(), std::chrono::system_clock::now());
}

} // namespace svetit::space::table
