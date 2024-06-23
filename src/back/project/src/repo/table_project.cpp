#include "table_project.hpp"
#include <cctype>
#include <exception>
#include <shared/errors.hpp>
#include "../model/sync_direction.hpp"

#include <userver/utils/boost_uuid4.hpp>
#include <userver/components/component_config.hpp>
#include <userver/components/component_context.hpp>
#include <userver/yaml_config/merge_schemas.hpp>
#include <userver/storages/postgres/component.hpp>

namespace svetit::project::table {

namespace pg = storages::postgres;
using pg::ClusterHostType;

Project::Project(pg::ClusterPtr pg)
	: db::Table<model::Project>{std::make_shared<db::Base>(std::move(pg))}
{}

const pg::Query kGet{
	"SELECT id, space_id, key, name, description, changed_at, sync FROM project.project WHERE id = $1 AND space_id = $2",
	pg::Query::Name{"select_project_by_id"},
};

model::Project Project::Get(const boost::uuids::uuid& spaceId, const boost::uuids::uuid& id) {
	auto res = _db->Execute(ClusterHostType::kMaster, kGet, id, spaceId);
	if (res.IsEmpty())
		throw errors::NotFound404{};

	return res.AsSingleRow<model::Project>(pg::kRowTag);
}

const pg::Query kSelectByKey{
	"SELECT id, space_id, key, name, description, changed_at, sync FROM project.project WHERE key = $1 AND space_id = $2",
	pg::Query::Name{"select_project_by_key"},
};

model::Project Project::GetByKey(const boost::uuids::uuid& spaceId, const std::string& key) {
	auto res = _db->Execute(ClusterHostType::kMaster, kSelectByKey, key, spaceId);
	if (res.IsEmpty())
		throw errors::NotFound404{};

	return res.AsSingleRow<model::Project>(pg::kRowTag);
}

const pg::Query kCreate{
	"INSERT INTO project.project (space_id, key, name, description, changed_at, sync) "
	"VALUES ($1, $2, $3, $4, $5, $6) "
	"RETURNING id",
	pg::Query::Name{"insert_project"},
};

boost::uuids::uuid Project::Create(const model::Project& item)
{
	auto res = _db->Execute(ClusterHostType::kMaster, kCreate, item.spaceId, item.key, item.name, item.description, item.changedAt, item.sync);
	return res.AsSingleRow<boost::uuids::uuid>();
}

const pg::Query kUpdate {
	"UPDATE project.project SET key = $3, name = $4, description = $5, changed_at = $6, sync = $7 "
	"WHERE id = $1 AND space_id = $2",
	pg::Query::Name{"update_project"},
};

void Project::Update(const model::Project& item) {
	auto res = _db->Execute(ClusterHostType::kMaster, kUpdate, item.id, item.spaceId, item.key, item.name, item.description, item.changedAt, item.sync);
	if (!res.RowsAffected())
		throw errors::NotFound404();
}

const pg::Query kDelete {
	"DELETE FROM project.project WHERE id = $1 AND space_id = $2",
	pg::Query::Name{"delete_project"},
};

void Project::Delete(const boost::uuids::uuid& spaceId, const boost::uuids::uuid& id) {
	auto res = _db->Execute(ClusterHostType::kMaster, kDelete, id, spaceId);
	if (!res.RowsAffected())
		throw errors::NotFound404();
}

const pg::Query kSelectProjects{
	"SELECT id, space_id, key, name, description, changed_at, sync, COUNT(*) OVER() "
	"FROM project.project "
	"WHERE space_id = $1 "
	"OFFSET $2 LIMIT $3",
	pg::Query::Name{"select_projects"},
};

PagingResult<model::Project> Project::GetList(const boost::uuids::uuid& spaceId, int start, int limit) {
	LOG_ERROR() << "NAMES: " << TableFieldsString();
	LOG_ERROR() << "T Name: " << TableName();

	Delete(spaceId, spaceId);

	auto res = _db->Execute(ClusterHostType::kSlave, kSelectProjects, spaceId, start, limit);

	PagingResult<model::Project> data;
	data = res.AsContainer<decltype(data)::RawContainer>(pg::kRowTag);

	if (!data.items.empty())
	{
		model::Project item = data.items.front();
		item.key += "1";
		try {
		auto tt = Create2(item);
		LOG_ERROR() << " TT= " << utils::ToString(tt);
		} catch (const std::exception& e) {
			LOG_ERROR() << "!ASD std ERR " << e.what();
		} catch (...) {
			LOG_ERROR() << "!ASD2";
		}
	}
	return data;
}

} // namespace svetit::project::table
