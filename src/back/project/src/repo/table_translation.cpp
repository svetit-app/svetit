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
	"SELECT id, project_id, lang, key, value FROM project.translation WHERE id = $1",
	pg::Query::Name{"select_translation"},
};

model::Translation Translation::Get(int64_t id) {
	auto res = _pg->Execute(ClusterHostType::kMaster, kGet, id);
	if (res.IsEmpty())
		throw errors::NotFound404{};

	return res.AsSingleRow<model::Translation>(pg::kRowTag);
}

const pg::Query kInsert{
	"INSERT INTO project.translation (project_id, lang, key, value) "
	"VALUES ($1, $2, $3, $4)",
	pg::Query::Name{"insert_translation"},
};

void Translation::Insert(
		const boost::uuids::uuid& projectId,
		const std::string& lang,
		const std::string& key,
		const std::string& value)
{
	_pg->Execute(ClusterHostType::kMaster, kInsert, projectId, lang, key, value);
}

const pg::Query kUpdate {
	"UPDATE project.translation SET project_id = $2, lang = $3, key = $4, value = $5 "
	"WHERE id = $1",
	pg::Query::Name{"update_translation"},
};

void Translation::Update(const model::Translation& translation) {
	auto res = _pg->Execute(ClusterHostType::kMaster, kUpdate, translation.id, translation.projectId, translation.lang, translation.key, translation.value);
	if (!res.RowsAffected())
		throw errors::NotFound404();
}

const pg::Query kDelete {
	"DELETE FROM project.translation WHERE id = $1",
	pg::Query::Name{"delete_translation"},
};

void Translation::Delete(int64_t id) {
	auto res = _pg->Execute(ClusterHostType::kMaster, kDelete, id);
	if (!res.RowsAffected())
		throw errors::NotFound404();
}

const pg::Query kSelectTranslations{
	"SELECT id, project_id, lang, key, value FROM project.translation "
	"OFFSET $1 LIMIT $2",
	pg::Query::Name{"select_translations"},
};

const pg::Query kCount{
	"SELECT COUNT(*) FROM project.translation",
	pg::Query::Name{"count_translations"},
};

PagingResult<model::Translation> Translation::GetList(int start, int limit) {
	PagingResult<model::Translation> data;

	auto trx = _pg->Begin(pg::Transaction::RO);
	auto res = trx.Execute(kSelectTranslations, start, limit);
	data.items = res.AsContainer<decltype(data.items)>(pg::kRowTag);
	res = trx.Execute(kCount);
	data.total = res.AsSingleRow<int64_t>();
	trx.Commit();
	return data;
}

} // namespace svetit::project::table
