#include "table_space.hpp"
#include "../../../shared/errors.hpp"
#include "../../../shared/paging.hpp"
#include <chrono>

#include <userver/components/component_config.hpp>
#include <userver/components/component_context.hpp>
#include <userver/utils/boost_uuid4.hpp>
#include <userver/yaml_config/merge_schemas.hpp>
#include <userver/storages/postgres/component.hpp>

#include <boost/uuid/uuid_io.hpp>

namespace svetit::space::table {

namespace pg = storages::postgres;
using pg::ClusterHostType;

Space::Space(pg::ClusterPtr pg)
	: _pg{std::move(pg)}
{
	//InsertDataForMocks();
}

const pg::Query kInsertSpace{
	"INSERT INTO space.space (id, name, key, requestsAllowed) "
	"VALUES ($1, $2, $3, $4) ",
	pg::Query::Name{"insert_space"},
};

void Space::Insert(const boost::uuids::uuid& id, const std::string& name, const std::string& key, bool requestsAllowed)
{
	_pg->Execute(ClusterHostType::kMaster, kInsertSpace, id, name, key, requestsAllowed);
}

const pg::Query kSelectSpaceAvailable{
	R"~(
		SELECT s.id, s.name, s.key, s.requestsAllowed, s.createdAt
		FROM space.space s
		LEFT JOIN space.user su ON s.id = su.spaceId AND su.userId = $1
		WHERE s.requestsAllowed = true AND su.spaceId IS NULL
		OFFSET $2 LIMIT $3;
	)~",
	pg::Query::Name{"select_space_available"},
};

const pg::Query kCountSpaceAvailable{
	R"~(
		SELECT COUNT(*)
		FROM space.space s
		LEFT JOIN space.user su ON s.id = su.spaceId AND su.userId = $1
		WHERE s.requestsAllowed = true AND su.spaceId IS NULL;
	)~",
	pg::Query::Name{"count_space_available"},
};

PagingResult<model::Space> Space::SelectAvailable(const std::string& userId, const int offset, const int limit)
{
	PagingResult<model::Space> data;

	auto trx = _pg->Begin(pg::Transaction::RO);
	auto res = trx.Execute(kSelectSpaceAvailable, userId, offset, limit);
	data.items = res.AsContainer<decltype(data.items)>(pg::kRowTag);
	res = trx.Execute(kCountSpaceAvailable, userId);
	data.total = res.AsSingleRow<int64_t>();
	trx.Commit();
	return data;
}

const pg::Query kSelectByUserId{
	R"~(
		SELECT s.id, s.name, s.key, s.requestsAllowed, s.createdAt
		FROM space.space s
		LEFT JOIN space.user su ON s.id = su.spaceId
		WHERE su.userId = $1
		OFFSET $2 LIMIT $3
	)~",
	pg::Query::Name{"select_space_by_user_id"},
};

const pg::Query kCountByUserId{
	R"~(
		SELECT COUNT(*)
		FROM space.space s
		LEFT JOIN space.user su ON s.id = su.spaceId
		WHERE su.userId = $1
	)~",
	pg::Query::Name{"count_space_by_user_id"},
};

PagingResult<model::Space> Space::SelectByUserId(const std::string& userId, const int offset, const int limit)
{
	PagingResult<model::Space> data;

	auto trx = _pg->Begin(pg::Transaction::RO);
	auto res = trx.Execute(kSelectByUserId, userId, offset, limit);
	data.items = res.AsContainer<decltype(data.items)>(pg::kRowTag);
	res = trx.Execute(kCountByUserId, userId);
	data.total = res.AsSingleRow<int64_t>();
	trx.Commit();
	return data;
}

const pg::Query kSelectSpaceByKey{
	"SELECT 1 FROM space.space WHERE key=$1",
	pg::Query::Name{"select_space_by_key"},
};

bool Space::IsExists(const std::string& key) {
	auto res = _pg->Execute(ClusterHostType::kMaster, kSelectSpaceByKey, key);
	return !res.IsEmpty();
}

const pg::Query kSelectWithDateClauseForOwner {
	R"~(
		SELECT 1
		FROM space.space s
		LEFT JOIN space.user su ON s.id = su.spaceId
		WHERE s.createdAt >= $1 AND su.userId = $2 AND su.isOwner = true
	)~",
	pg::Query::Name{"select_space_with_date_clause_for_owner"},
};

bool Space::IsReadyForCreationByTime(const std::string& userId) {
	const auto minuteAgo = std::chrono::system_clock::now() - std::chrono::minutes(1);
	const auto minuteAgoTimestamp = std::chrono::duration_cast<std::chrono::seconds>(minuteAgo.time_since_epoch()).count();

	auto res = _pg->Execute(ClusterHostType::kMaster, kSelectWithDateClauseForOwner, minuteAgoTimestamp, userId);
	return res.IsEmpty();
}

const pg::Query kCountSpacesWithUser {
	R"~(
		SELECT COUNT(*)
		FROM space.space s
		LEFT JOIN space.user su ON s.id = su.spaceId
		WHERE su.userId = $1
	)~",
	pg::Query::Name{"count_spaces_with_user"},
};

int64_t Space::GetCountSpacesWithUser(const std::string& userId) {
	const auto res = _pg->Execute(ClusterHostType::kMaster, kCountSpacesWithUser, userId);
	if (res.IsEmpty())
		return 0;

	return res.AsSingleRow<int64_t>();
}

const pg::Query kDelete {
	"DELETE FROM space.space WHERE id = $1",
	pg::Query::Name{"delete_space"},
};

void Space::Delete(const boost::uuids::uuid& spaceUuid) {
	auto res = _pg->Execute(ClusterHostType::kMaster, kDelete, spaceUuid);
	if (!res.RowsAffected())
		throw errors::NotFound();
}

const pg::Query kSelectById{
	"SELECT id, name, key, requestsAllowed, createdAt FROM space.space WHERE id = $1",
	pg::Query::Name{"select_by_id"},
};

model::Space Space::SelectById(const boost::uuids::uuid& id) {
	auto res = _pg->Execute(ClusterHostType::kMaster, kSelectById, id);
	if (res.IsEmpty())
		throw errors::NotFound{};

	return res.AsSingleRow<model::Space>(pg::kRowTag);
}

const pg::Query kSelectByKey{
	"SELECT id, name, key, requestsAllowed, createdAt FROM space.space WHERE key = $1",
	pg::Query::Name{"select_by_key"},
};

model::Space Space::SelectByKey(const std::string& key) {
	auto res = _pg->Execute(ClusterHostType::kMaster, kSelectByKey, key);
	if (res.IsEmpty())
		throw errors::NotFound{};

	return res.AsSingleRow<model::Space>(pg::kRowTag);
}

void Space::InsertDataForMocks() {
	// insert test data
	Insert(utils::BoostUuidFromString("11111111-1111-1111-1111-111111111111"), "Пространство №1", "key1", true);
	Insert(utils::BoostUuidFromString("22222222-2222-2222-2222-222222222222"), "Пространство №2", "key2", true);
	Insert(utils::BoostUuidFromString("33333333-3333-3333-3333-333333333333"), "Пространство №3", "key3", true);
	Insert(utils::BoostUuidFromString("44444444-4444-4444-4444-444444444444"), "Пространство №4", "key4", true);
	Insert(utils::BoostUuidFromString("55555555-5555-5555-5555-555555555555"), "Пространство №5", "key5", true);
	Insert(utils::BoostUuidFromString("66666666-6666-6666-6666-666666666666"), "Пространство №6", "key6", true);
	Insert(utils::BoostUuidFromString("77777777-7777-7777-7777-777777777777"), "Пространство №7", "key7", true);
	Insert(utils::BoostUuidFromString("88888888-8888-8888-8888-888888888888"), "Пространство №8", "key8", true);
	Insert(utils::BoostUuidFromString("99999999-9999-9999-9999-999999999999"), "Пространство №9", "key9", true);
	Insert(utils::BoostUuidFromString("10000000-1000-1000-1000-100000000000"), "Пространство №10", "key10", true);
	Insert(utils::BoostUuidFromString("11000000-1100-1100-1100-110000000000"), "Пространство №11", "key11", true);
}

} // namespace svetit::space::table
