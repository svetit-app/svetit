#include "table_code.hpp"
#include <shared/errors.hpp>
#include <shared/paging.hpp>

#include <userver/components/component_config.hpp>
#include <userver/components/component_context.hpp>
#include <userver/yaml_config/merge_schemas.hpp>
#include <userver/storages/postgres/component.hpp>

namespace svetit::project::table {

namespace pg = storages::postgres;
using pg::ClusterHostType;

Code::Code(pg::ClusterPtr pg)
	: _pg{std::move(pg)}
{}

const pg::Query kGet{
	"SELECT id, project_id, repository_id, commit_hash FROM project.code WHERE id = $1",
	pg::Query::Name{"select_code"},
};

model::Code Code::Get(int64_t id) {
	auto res = _pg->Execute(ClusterHostType::kMaster, kGet, id);
	if (res.IsEmpty())
		throw errors::NotFound404{};

	return res.AsSingleRow<model::Code>(pg::kRowTag);
}

const pg::Query kCreate{
	"INSERT INTO project.code (project_id, repository_id, commit_hash) "
	"VALUES ($1, $2, $3)"
	"RETURNING id",
	pg::Query::Name{"insert_code"},
};

int64_t Code::Create(const model::Code& code)
{
	auto res = _pg->Execute(ClusterHostType::kMaster, kCreate, code.projectId, code.repositoryId, code.commitHash);
	return res.AsSingleRow<int64_t>();
}

const pg::Query kUpdate {
	"UPDATE project.code SET project_id = $2, repository_id = $3, commit_hash = $4 "
	"WHERE id = $1",
	pg::Query::Name{"update_code"},
};

void Code::Update(const model::Code& code) {
	auto res = _pg->Execute(ClusterHostType::kMaster, kUpdate, code.id, code.projectId, code.repositoryId, code.commitHash);
	if (!res.RowsAffected())
		throw errors::NotFound404();
}

const pg::Query kDelete {
	"DELETE FROM project.code WHERE id = $1",
	pg::Query::Name{"delete_code"},
};

void Code::Delete(int64_t id) {
	auto res = _pg->Execute(ClusterHostType::kMaster, kDelete, id);
	if (!res.RowsAffected())
		throw errors::NotFound404();
}

const pg::Query kSelectCodes{
	"SELECT id, project_id, repository_id, commit_hash FROM project.code "
	"OFFSET $1 LIMIT $2",
	pg::Query::Name{"select_codes"},
};

const pg::Query kCount{
	"SELECT COUNT(*) FROM project.code",
	pg::Query::Name{"count_codes"},
};

PagingResult<model::Code> Code::GetList(int start, int limit) {
	PagingResult<model::Code> data;

	auto trx = _pg->Begin(pg::Transaction::RO);
	auto res = trx.Execute(kSelectCodes, start, limit);
	data.items = res.AsContainer<decltype(data.items)>(pg::kRowTag);
	res = trx.Execute(kCount);
	data.total = res.AsSingleRow<int64_t>();
	trx.Commit();
	return data;
}

} // namespace svetit::project::table
