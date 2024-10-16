#include "table_space_link.hpp"
#include <svetit/errors.hpp>
#include <svetit/paging.hpp>
#include <chrono>
#include <memory>

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

SpaceLink::SpaceLink(std::shared_ptr<db::Base> dbPtr)
	: _db{std::move(dbPtr)}
{}

const pg::Query kInsertSpaceLink{
	"INSERT INTO space.link (space_id, creator_id, name, expired_at) "
	"VALUES ($1, $2, $3, $4) ",
	pg::Query::Name{"insert_space.link"},
};

void SpaceLink::Insert(
	const boost::uuids::uuid& spaceId,
	const std::string& creatorId,
	const std::string& name,
	std::chrono::system_clock::time_point expiredAt)
{
	_db->Execute(ClusterHostType::kMaster, kInsertSpaceLink, spaceId, creatorId, name, expiredAt);
}

const pg::Query kSelectSpaceLinkBySpace{
	"SELECT id, space_id, creator_id, name, created_at, expired_at, COUNT(*) OVER() "
	"FROM space.link WHERE space_id=$1 OFFSET $2 LIMIT $3",
	pg::Query::Name{"select_space.link_by_space"},
};

PagingResult<model::SpaceLink> SpaceLink::SelectBySpace(const boost::uuids::uuid& spaceId, int offset, int limit)
{
	auto res = _db->Execute(ClusterHostType::kSlave, kSelectSpaceLinkBySpace, spaceId, offset, limit);
	PagingResult<model::SpaceLink> data;
	data = res.AsContainer<decltype(data)::RawContainer>(pg::kRowTag);
	return data;
}

const pg::Query kDeleteBySpace {
	"DELETE FROM space.link WHERE space_id = $1",
	pg::Query::Name{"delete_space.link_by_space"},
};

void SpaceLink::DeleteBySpace(const boost::uuids::uuid& spaceId) {
	_db->Execute(ClusterHostType::kMaster, kDeleteBySpace, spaceId);
}

const pg::Query kDeleteById {
	"DELETE FROM space.link WHERE id = $1",
	pg::Query::Name{"delete_space.link_by_id"},
};

void SpaceLink::DeleteById(const boost::uuids::uuid& id) {
	auto res = _db->Execute(ClusterHostType::kMaster, kDeleteById, id);
	if (!res.RowsAffected())
		throw errors::NotFound404();
}

const pg::Query kSelectById{
	"SELECT id, space_id, creator_id, name, created_at, expired_at FROM space.link WHERE id = $1",
	pg::Query::Name{"select_by_id"},
};

model::SpaceLink SpaceLink::SelectById(const boost::uuids::uuid& id) {
	auto res = _db->Execute(ClusterHostType::kSlave, kSelectById, id);
	if (res.IsEmpty())
		throw errors::NotFound404{};

	return res.AsSingleRow<model::SpaceLink>(pg::kRowTag);
}

} // namespace svetit::space::table
