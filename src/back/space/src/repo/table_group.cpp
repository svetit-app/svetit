#include "table_group.hpp"
#include <memory>
#include <shared/errors.hpp>
#include <shared/paging.hpp>

#include <userver/components/component_config.hpp>
#include <userver/components/component_context.hpp>
#include <userver/utils/boost_uuid4.hpp>
#include <userver/yaml_config/merge_schemas.hpp>
#include <userver/storages/postgres/component.hpp>

#include <boost/uuid/uuid_io.hpp>

namespace svetit::space::table {

namespace pg = storages::postgres;
using pg::ClusterHostType;

Group::Group(std::shared_ptr<db::Base> dbPtr)
	: _db{std::move(dbPtr)}
{}

const pg::Query kSelect{
	"SELECT id, name, description, space_id FROM space.group WHERE id = $1",
	pg::Query::Name{"group.select"},
};

model::Group Group::Select(int id) {
	auto res = _db->Execute(ClusterHostType::kSlave, kSelect, id);
	if (res.IsEmpty())
		throw errors::NotFound404{};

	return res.AsSingleRow<model::Group>(pg::kRowTag);
}

const pg::Query kInsert{
	"INSERT INTO space.group (name, description, space_id) "
	"VALUES ($1, $2, $3) RETURNING id",
	pg::Query::Name{"group.insert"},
};

int Group::Create(const model::Group& item) {
	const auto res = _db->Execute(ClusterHostType::kMaster, kInsert, item.name, item.description, item.spaceId);
	return res.AsSingleRow<int>();
}

const pg::Query kDelete {
	"DELETE FROM space.group WHERE id = $1",
	pg::Query::Name{"group.delete"},
};

void Group::Delete(int id) {
	auto res = _db->Execute(ClusterHostType::kMaster, kDelete, id);
	if (!res.RowsAffected())
		throw errors::NotFound404();
}

const pg::Query kUpdate {
	"UPDATE space.group SET name = $2, description = $3, space_id = $4 "
	"WHERE id = $1",
	pg::Query::Name{"group.update"},
};

void Group::Update(const model::Group& item) {
	auto res = _db->Execute(ClusterHostType::kMaster, kUpdate, item.id, item.name, item.description, item.spaceId);
	if (!res.RowsAffected())
		throw errors::NotFound404();
}

const pg::Query kSelectList{
	"SELECT id, name, description, space_id, COUNT(*) OVER() FROM space.group OFFSET $1 LIMIT $2",
	pg::Query::Name{"group.select_list"},
};

PagingResult<model::Group> Group::SelectList(int32_t start, int32_t limit) {
	auto res = _db->Execute(ClusterHostType::kSlave, kSelectList, start, limit);
	PagingResult<model::Group> data;
	data = res.AsContainer<decltype(data)::RawContainer>(pg::kRowTag);
	return data;
}

const pg::Query kSelectListBySpaceId {
	"SELECT id, name, description, space_id, COUNT(*) OVER() FROM space.group WHERE space_id = $1 OFFSET $2 LIMIT $3",
	pg::Query::Name{"space.select_group_list_by_spaceId"},
};

PagingResult<model::Group> Group::SelectListBySpaceId(int32_t start, int32_t limit, const boost::uuids::uuid& spaceId) {
	auto res = _db->Execute(ClusterHostType::kSlave, kSelectListBySpaceId, spaceId, start, limit);
	PagingResult<model::Group> data;
	data = res.AsContainer<decltype(data)::RawContainer>(pg::kRowTag);
	return data;
}

} // namespace svetit::space::table
