#include "table_space_link.hpp"
#include "../../../shared/errors.hpp"

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
	id UUID PRIMARY KEY,
	spaceId UUID NOT NULL,
	creatorId TEXT NOT NULL,
	name TEXT NOT NULL,
	createdAt BIGINT NOT NULL,
	expiredAt BIGINT NOT NULL,
	FOREIGN KEY (spaceId) REFERENCES space (id)
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
	const std::string& creatorId,
	const std::string& name,
	int64_t createdAt,
	int64_t expiredAt)
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

const storages::postgres::Query kGetSpaceIdById{
	"SELECT spaceId FROM space_link WHERE id = $1",
	storages::postgres::Query::Name{"count_space_link"},
};

boost::uuids::uuid SpaceLink::GetSpaceId(boost::uuids::uuid id) {
	storages::postgres::Transaction transaction =
		_pg->Begin("count_space_link_transaction",
			storages::postgres::ClusterHostType::kMaster, {});

	auto res = transaction.Execute(kGetSpaceIdById, id);

	boost::uuids::uuid spaceId;

	if (!res.IsEmpty()) {
		spaceId = res.Front()[0].As<boost::uuids::uuid>();
	}

	transaction.Commit();

	return spaceId;
}

const storages::postgres::Query kSelectById{
	"SELECT * FROM space_link WHERE id = $1",
	storages::postgres::Query::Name{"select_by_id"},
};

model::SpaceLink SpaceLink::SelectById(boost::uuids::uuid id) {
	storages::postgres::Transaction transaction =
		_pg->Begin("select_space_link_by_id_transaction",
			storages::postgres::ClusterHostType::kMaster, {});

	auto res = transaction.Execute(kSelectById, id);

	if (res.IsEmpty())
	{
		transaction.Commit();
		throw errors::BadRequestException{"Not found"};
		return {};
	}

	transaction.Commit();
	return res.AsSingleRow<model::SpaceLink>(pg::kRowTag);
}

void SpaceLink::InsertDataForMocks() {
	const auto p1 = std::chrono::system_clock::now();
	const auto now = std::chrono::duration_cast<std::chrono::seconds>(p1.time_since_epoch()).count();
	Insert(boost::uuids::random_generator()(), utils::BoostUuidFromString("11111111-1111-1111-1111-111111111111"), "8ad16a1d-18b1-4aaa-8b0f-f61915974c66", "link1", now, now);
	Insert(boost::uuids::random_generator()(), utils::BoostUuidFromString("22222222-2222-2222-2222-222222222222"), "8ad16a1d-18b1-4aaa-8b0f-f61915974c66", "link2", now, now);
	Insert(boost::uuids::random_generator()(), utils::BoostUuidFromString("33333333-3333-3333-3333-333333333333"), "8ad16a1d-18b1-4aaa-8b0f-f61915974c66", "link3", now, now);
	Insert(boost::uuids::random_generator()(), utils::BoostUuidFromString("44444444-4444-4444-4444-444444444444"), "8ad16a1d-18b1-4aaa-8b0f-f61915974c66", "link4", now, now);
	Insert(boost::uuids::random_generator()(), utils::BoostUuidFromString("55555555-5555-5555-5555-555555555555"), "8ad16a1d-18b1-4aaa-8b0f-f61915974c66", "link5", now, now);
	Insert(boost::uuids::random_generator()(), utils::BoostUuidFromString("66666666-6666-6666-6666-666666666666"), "8ad16a1d-18b1-4aaa-8b0f-f61915974c66", "link6", now, now);
	Insert(boost::uuids::random_generator()(), utils::BoostUuidFromString("77777777-7777-7777-7777-777777777777"), "8ad16a1d-18b1-4aaa-8b0f-f61915974c66", "link7", now, now);
	Insert(boost::uuids::random_generator()(), utils::BoostUuidFromString("88888888-8888-8888-8888-888888888888"), "8ad16a1d-18b1-4aaa-8b0f-f61915974c66", "link8", now, now);
	Insert(boost::uuids::random_generator()(), utils::BoostUuidFromString("11111111-1111-1111-1111-111111111111"), "8ad16a1d-18b1-4aaa-8b0f-f61915974c66", "link11", now, now);
	Insert(boost::uuids::random_generator()(), utils::BoostUuidFromString("11111111-1111-1111-1111-111111111111"), "8ad16a1d-18b1-4aaa-8b0f-f61915974c66", "link2", now, now);
	Insert(boost::uuids::random_generator()(), utils::BoostUuidFromString("11111111-1111-1111-1111-111111111111"), "8ad16a1d-18b1-4aaa-8b0f-f61915974c66", "link3", now, now);
	Insert(boost::uuids::random_generator()(), utils::BoostUuidFromString("11111111-1111-1111-1111-111111111111"), "8ad16a1d-18b1-4aaa-8b0f-f61915974c66", "link4", now, now);
	Insert(boost::uuids::random_generator()(), utils::BoostUuidFromString("11111111-1111-1111-1111-111111111111"), "8ad16a1d-18b1-4aaa-8b0f-f61915974c66", "link5", now, now);
	Insert(boost::uuids::random_generator()(), utils::BoostUuidFromString("11111111-1111-1111-1111-111111111111"), "8ad16a1d-18b1-4aaa-8b0f-f61915974c66", "link6", now, now);
	Insert(boost::uuids::random_generator()(), utils::BoostUuidFromString("11111111-1111-1111-1111-111111111111"), "8ad16a1d-18b1-4aaa-8b0f-f61915974c66", "link7", now, now);
	Insert(boost::uuids::random_generator()(), utils::BoostUuidFromString("11111111-1111-1111-1111-111111111111"), "8ad16a1d-18b1-4aaa-8b0f-f61915974c66", "link8", now, now);
}

} // namespace svetit::space::table
