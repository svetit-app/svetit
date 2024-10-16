#include "table_space.hpp"
#include <memory>
#include <svetit/errors.hpp>
#include <svetit/paging.hpp>

#include <userver/components/component_config.hpp>
#include <userver/components/component_context.hpp>
#include <userver/utils/boost_uuid4.hpp>
#include <userver/yaml_config/merge_schemas.hpp>
#include <userver/storages/postgres/component.hpp>

#include <boost/uuid/uuid_io.hpp>

namespace svetit::space::table {

namespace pg = storages::postgres;
using pg::ClusterHostType;

Space::Space(std::shared_ptr<db::Base> dbPtr)
	: _db{std::move(dbPtr)}
{}

const pg::Query kInsertSpace{
	"INSERT INTO space.space (name, key, requests_allowed) "
	"VALUES ($1, $2, $3) RETURNING id",
	pg::Query::Name{"insert_space"},
};

boost::uuids::uuid Space::Create(const std::string& name, const std::string& key, bool requestsAllowed)
{
	const auto res = _db->Execute(ClusterHostType::kMaster, kInsertSpace, name, key, requestsAllowed);
	return res.AsSingleRow<boost::uuids::uuid>();
}

const pg::Query kSelectSpaceByKey{
	"SELECT 1 FROM space.space WHERE key=$1",
	pg::Query::Name{"select_space_by_key"},
};

bool Space::IsExists(const std::string& key) {
	auto res = _db->Execute(ClusterHostType::kSlave, kSelectSpaceByKey, key);
	return !res.IsEmpty();
}

const pg::Query kDelete {
	"DELETE FROM space.space WHERE id = $1",
	pg::Query::Name{"delete_space"},
};

void Space::Delete(const boost::uuids::uuid& spaceId) {
	auto res = _db->Execute(ClusterHostType::kMaster, kDelete, spaceId);
	if (!res.RowsAffected())
		throw errors::NotFound404();
}

const pg::Query kSelectById{
	"SELECT id, name, key, requests_allowed, created_at FROM space.space WHERE id = $1",
	pg::Query::Name{"select_by_id"},
};

model::Space Space::SelectById(const boost::uuids::uuid& id) {
	auto res = _db->Execute(ClusterHostType::kSlave, kSelectById, id);
	if (res.IsEmpty())
		throw errors::NotFound404{};

	return res.AsSingleRow<model::Space>(pg::kRowTag);
}

const pg::Query kSelectByKey{
	"SELECT id, name, key, requests_allowed, created_at FROM space.space WHERE key = $1",
	pg::Query::Name{"select_by_key"},
};

model::Space Space::SelectByKey(const std::string& key) {
	auto res = _db->Execute(ClusterHostType::kSlave, kSelectByKey, key);
	if (res.IsEmpty())
		throw errors::NotFound404{};

	return res.AsSingleRow<model::Space>(pg::kRowTag);
}

} // namespace svetit::space::table
