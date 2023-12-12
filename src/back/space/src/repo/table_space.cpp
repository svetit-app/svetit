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
	"INSERT INTO space.space (name, key, requestsAllowed) "
	"VALUES ($1, $2, $3) RETURNING id",
	pg::Query::Name{"insert_space"},
};

boost::uuids::uuid Space::Insert(const std::string& name, const std::string& key, bool requestsAllowed)
{
	const auto res =_pg->Execute(ClusterHostType::kMaster, kInsertSpace, name, key, requestsAllowed);
	return res.AsSingleRow<boost::uuids::uuid>();
}

const pg::Query kSelectSpaceByKey{
	"SELECT 1 FROM space.space WHERE key=$1",
	pg::Query::Name{"select_space_by_key"},
};

bool Space::IsExists(const std::string& key) {
	auto res = _pg->Execute(ClusterHostType::kMaster, kSelectSpaceByKey, key);
	return !res.IsEmpty();
}

const pg::Query kDelete {
	"DELETE FROM space.space WHERE id = $1",
	pg::Query::Name{"delete_space"},
};

void Space::Delete(const boost::uuids::uuid& spaceId) {
	auto res = _pg->Execute(ClusterHostType::kMaster, kDelete, spaceId);
	if (!res.RowsAffected())
		throw errors::NotFound404();
}

const pg::Query kSelectById{
	"SELECT id, name, key, requestsAllowed, createdAt FROM space.space WHERE id = $1",
	pg::Query::Name{"select_by_id"},
};

model::Space Space::SelectById(const boost::uuids::uuid& id) {
	auto res = _pg->Execute(ClusterHostType::kMaster, kSelectById, id);
	if (res.IsEmpty())
		throw errors::NotFound404{};

	return res.AsSingleRow<model::Space>(pg::kRowTag);
}

const pg::Query kSelectByKey{
	"SELECT id, name, key, requestsAllowed, createdAt FROM space.space WHERE key = $1",
	pg::Query::Name{"select_by_key"},
};

model::Space Space::SelectByKey(const std::string& key) {
	auto res = _pg->Execute(ClusterHostType::kMaster, kSelectByKey, key);
	if (res.IsEmpty())
		throw errors::NotFound404{};

	return res.AsSingleRow<model::Space>(pg::kRowTag);
}

void Space::InsertDataForMocks() {
	// insert test data
	const pg::Query kInsertSpaceForMocks{
		"INSERT INTO space.space (id, name, key, requestsAllowed) "
		"VALUES ($1, $2, $3, $4)",
		pg::Query::Name{"insert_space_for_mocks"},
	};

	_pg->Execute(ClusterHostType::kMaster, kInsertSpaceForMocks, utils::BoostUuidFromString("11111111-1111-1111-1111-111111111111"), "Пространство №1", "key1", true);
	_pg->Execute(ClusterHostType::kMaster, kInsertSpaceForMocks, utils::BoostUuidFromString("22222222-2222-2222-2222-222222222222"), "Пространство №2", "key2", true);
	_pg->Execute(ClusterHostType::kMaster, kInsertSpaceForMocks, utils::BoostUuidFromString("33333333-3333-3333-3333-333333333333"), "Пространство №3", "key3", true);
	_pg->Execute(ClusterHostType::kMaster, kInsertSpaceForMocks, utils::BoostUuidFromString("44444444-4444-4444-4444-444444444444"), "Пространство №4", "key4", true);
	_pg->Execute(ClusterHostType::kMaster, kInsertSpaceForMocks, utils::BoostUuidFromString("55555555-5555-5555-5555-555555555555"), "Пространство №5", "key5", true);
	_pg->Execute(ClusterHostType::kMaster, kInsertSpaceForMocks, utils::BoostUuidFromString("66666666-6666-6666-6666-666666666666"), "Пространство №6", "key6", true);
	_pg->Execute(ClusterHostType::kMaster, kInsertSpaceForMocks, utils::BoostUuidFromString("77777777-7777-7777-7777-777777777777"), "Пространство №7", "key7", true);
	_pg->Execute(ClusterHostType::kMaster, kInsertSpaceForMocks, utils::BoostUuidFromString("88888888-8888-8888-8888-888888888888"), "Пространство №8", "key8", true);
	_pg->Execute(ClusterHostType::kMaster, kInsertSpaceForMocks, utils::BoostUuidFromString("99999999-9999-9999-9999-999999999999"), "Пространство №9", "key9", true);
	_pg->Execute(ClusterHostType::kMaster, kInsertSpaceForMocks, utils::BoostUuidFromString("10000000-1000-1000-1000-100000000000"), "Пространство №10", "key10", true);
	_pg->Execute(ClusterHostType::kMaster, kInsertSpaceForMocks, utils::BoostUuidFromString("11000000-1100-1100-1100-110000000000"), "Пространство №11", "key11", true);
}

} // namespace svetit::space::table
