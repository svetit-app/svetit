#include "table_translation.hpp"
#include <shared/errors.hpp>
#include <shared/paging.hpp>

#include <userver/components/component_config.hpp>
#include <userver/components/component_context.hpp>
#include <userver/yaml_config/merge_schemas.hpp>
#include <userver/storages/postgres/component.hpp>

namespace svetit::project::table {

namespace pg = storages::postgres;
using pg::ClusterHostType;

Translation::Translation(pg::ClusterPtr pg)
	: _pg{std::move(pg)}
{}

const pg::Query kGet{
	"SELECT id, space_id, project_id, lang, key, value FROM project.translation WHERE id = $1 AND space_id = $2",
	pg::Query::Name{"select_translation"},
};

model::Translation Translation::Get(const boost::uuids::uuid& spaceId, int64_t id) {
	auto res = _pg->Execute(ClusterHostType::kMaster, kGet, id, spaceId);
	if (res.IsEmpty())
		throw errors::NotFound404{};

	return res.AsSingleRow<model::Translation>(pg::kRowTag);
}

const pg::Query kCreate{
	"INSERT INTO project.translation (space_id, project_id, lang, key, value) "
	"VALUES ($1, $2, $3, $4, $5)"
	"RETURNING id",
	pg::Query::Name{"insert_translation"},
};

int64_t Translation::Create(const model::Translation& translation)
{
	auto res = _pg->Execute(ClusterHostType::kMaster, kCreate, translation.spaceId, translation.projectId, translation.lang, translation.key, translation.value);
	return res.AsSingleRow<int64_t>();
}

const pg::Query kUpdate {
	"UPDATE project.translation SET project_id = $3, lang = $4, key = $5, value = $6 "
	"WHERE id = $1 AND space_id = $2",
	pg::Query::Name{"update_translation"},
};

void Translation::Update(const model::Translation& translation) {
	auto res = _pg->Execute(ClusterHostType::kMaster, kUpdate, translation.id, translation.spaceId, translation.projectId, translation.lang, translation.key, translation.value);
	if (!res.RowsAffected())
		throw errors::NotFound404();
}

const pg::Query kDelete {
	"DELETE FROM project.translation WHERE id = $1 AND space_id = $2",
	pg::Query::Name{"delete_translation"},
};

void Translation::Delete(const boost::uuids::uuid& spaceId, int64_t id) {
	auto res = _pg->Execute(ClusterHostType::kMaster, kDelete, id, spaceId);
	if (!res.RowsAffected())
		throw errors::NotFound404();
}

const pg::Query kSelectTranslations{
	"SELECT id, space_id, project_id, lang, key, value FROM project.translation "
	"WHERE space_id = $1 AND project_id = $2"
	"OFFSET $3 LIMIT $4",
	pg::Query::Name{"select_translations"},
};

const pg::Query kCount{
	"SELECT COUNT(*) FROM project.translation",
	pg::Query::Name{"count_translations"},
};

PagingResult<model::Translation> Translation::GetList(const boost::uuids::uuid& spaceId, const boost::uuids::uuid& projectId, int start, int limit) {
	PagingResult<model::Translation> data;

	auto trx = _pg->Begin(pg::Transaction::RO);
	auto res = trx.Execute(kSelectTranslations, spaceId, projectId, start, limit);
	data.items = res.AsContainer<decltype(data.items)>(pg::kRowTag);
	res = trx.Execute(kCount);
	data.total = res.AsSingleRow<int64_t>();
	trx.Commit();
	return data;
}

} // namespace svetit::project::table
