#include "table_space_link.hpp"
#include "../../../shared/errors.hpp"
#include "../../../shared/paging.hpp"
#include <chrono>

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
using pg::ClusterHostType;

SpaceLink::SpaceLink(pg::ClusterPtr pg)
	: _pg{std::move(pg)}
{
	//InsertDataForMocks();
}

const pg::Query kInsertSpaceLink{
	"INSERT INTO space.link (spaceId, creatorId, name, expiredAt) "
	"VALUES ($1, $2, $3, $4) ",
	pg::Query::Name{"insert_space.link"},
};

void SpaceLink::Insert(
	const boost::uuids::uuid& spaceId,
	const std::string& creatorId,
	const std::string& name,
	std::chrono::system_clock::time_point expiredAt)
{
	_pg->Execute(ClusterHostType::kMaster, kInsertSpaceLink, spaceId, creatorId, name, expiredAt);
}

const pg::Query kSelectSpaceLinkBySpace{
	"SELECT id, spaceId, creatorId, name, createdAt, expiredAt "
	"FROM space.link WHERE spaceId=$1 OFFSET $2 LIMIT $3",
	pg::Query::Name{"select_space.link_by_space"},
};

const pg::Query kCountSpaceLinkBySpace{
	"SELECT count(*) FROM space.link WHERE spaceId=$1",
	pg::Query::Name{"count_space.link_by_space"},
};

PagingResult<model::SpaceLink> SpaceLink::SelectBySpace(const boost::uuids::uuid& spaceId, int offset, int limit)
{
	PagingResult<model::SpaceLink> data;

	auto trx = _pg->Begin(pg::Transaction::RO);
	auto res = trx.Execute(kSelectSpaceLinkBySpace, spaceId, offset, limit);
	data.items = res.AsContainer<decltype(data.items)>(pg::kRowTag);
	res = trx.Execute(kCountSpaceLinkBySpace, spaceId);
	data.total = res.AsSingleRow<int64_t>();
	trx.Commit();
	return data;
}

const pg::Query kDeleteBySpace {
	"DELETE FROM space.link WHERE spaceId = $1",
	pg::Query::Name{"delete_space.link_by_space"},
};

void SpaceLink::DeleteBySpace(const boost::uuids::uuid& spaceId) {
	auto res = _pg->Execute(ClusterHostType::kMaster, kDeleteBySpace, spaceId);
	if (!res.RowsAffected())
		throw errors::NotFound404();
}

const pg::Query kDeleteById {
	"DELETE FROM space.link WHERE id = $1",
	pg::Query::Name{"delete_space.link_by_id"},
};

void SpaceLink::DeleteById(const boost::uuids::uuid& id) {
	auto res = _pg->Execute(ClusterHostType::kMaster, kDeleteById, id);
	if (!res.RowsAffected())
		throw errors::NotFound404();
}

const pg::Query kSelectById{
	"SELECT id, spaceId, creatorId, name, createdAt, expiredAt FROM space.link WHERE id = $1",
	pg::Query::Name{"select_by_id"},
};

model::SpaceLink SpaceLink::SelectById(const boost::uuids::uuid& id) {
	auto res = _pg->Execute(ClusterHostType::kMaster, kSelectById, id);
	if (res.IsEmpty())
		throw errors::NotFound404{};

	return res.AsSingleRow<model::SpaceLink>(pg::kRowTag);
}

void SpaceLink::InsertDataForMocks() {
	const auto now = std::chrono::system_clock::now();
	Insert(utils::BoostUuidFromString("11111111-1111-1111-1111-111111111111"), "8ad16a1d-18b1-4aaa-8b0f-f61915974c66", "link1", now);
	Insert(utils::BoostUuidFromString("22222222-2222-2222-2222-222222222222"), "8ad16a1d-18b1-4aaa-8b0f-f61915974c66", "link2", now);
	Insert(utils::BoostUuidFromString("33333333-3333-3333-3333-333333333333"), "8ad16a1d-18b1-4aaa-8b0f-f61915974c66", "link3", now);
	Insert(utils::BoostUuidFromString("44444444-4444-4444-4444-444444444444"), "8ad16a1d-18b1-4aaa-8b0f-f61915974c66", "link4", now);
	Insert(utils::BoostUuidFromString("55555555-5555-5555-5555-555555555555"), "8ad16a1d-18b1-4aaa-8b0f-f61915974c66", "link5", now);
	Insert(utils::BoostUuidFromString("66666666-6666-6666-6666-666666666666"), "8ad16a1d-18b1-4aaa-8b0f-f61915974c66", "link6", now);
	Insert(utils::BoostUuidFromString("77777777-7777-7777-7777-777777777777"), "8ad16a1d-18b1-4aaa-8b0f-f61915974c66", "link7", now);
	Insert(utils::BoostUuidFromString("88888888-8888-8888-8888-888888888888"), "8ad16a1d-18b1-4aaa-8b0f-f61915974c66", "link8", now);
	Insert(utils::BoostUuidFromString("11111111-1111-1111-1111-111111111111"), "8ad16a1d-18b1-4aaa-8b0f-f61915974c66", "link11", now);
	Insert(utils::BoostUuidFromString("11111111-1111-1111-1111-111111111111"), "8ad16a1d-18b1-4aaa-8b0f-f61915974c66", "link2", now);
	Insert(utils::BoostUuidFromString("11111111-1111-1111-1111-111111111111"), "8ad16a1d-18b1-4aaa-8b0f-f61915974c66", "link3", now);
	Insert(utils::BoostUuidFromString("11111111-1111-1111-1111-111111111111"), "8ad16a1d-18b1-4aaa-8b0f-f61915974c66", "link4", now);
	Insert(utils::BoostUuidFromString("11111111-1111-1111-1111-111111111111"), "8ad16a1d-18b1-4aaa-8b0f-f61915974c66", "link5", now);
	Insert(utils::BoostUuidFromString("11111111-1111-1111-1111-111111111111"), "8ad16a1d-18b1-4aaa-8b0f-f61915974c66", "link6", now);
	Insert(utils::BoostUuidFromString("11111111-1111-1111-1111-111111111111"), "8ad16a1d-18b1-4aaa-8b0f-f61915974c66", "link7", now);
	Insert(utils::BoostUuidFromString("11111111-1111-1111-1111-111111111111"), "8ad16a1d-18b1-4aaa-8b0f-f61915974c66", "link8", now);
}

} // namespace svetit::space::table
