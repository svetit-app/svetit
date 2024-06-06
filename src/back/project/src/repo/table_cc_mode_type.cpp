#include "table_cc_mode_type.hpp"
#include <shared/errors.hpp>
#include <shared/paging.hpp>

#include <userver/components/component_config.hpp>
#include <userver/components/component_context.hpp>
#include <userver/yaml_config/merge_schemas.hpp>
#include <userver/storages/postgres/component.hpp>

namespace svetit::project::table {

namespace pg = storages::postgres;
using pg::ClusterHostType;

CcModeType::CcModeType(pg::ClusterPtr pg)
	: _pg{std::move(pg)}
{}

const pg::Query kGet{
	"SELECT id, space_id, project_id, cc_type_id, key, name FROM project.cc_mode_type WHERE id = $1 AND space_id = $2",
	pg::Query::Name{"select_cc_mode_type"}
};

model::CcModeType CcModeType::Get(const boost::uuids::uuid& spaceId, int64_t id) {
	auto res = _pg->Execute(ClusterHostType::kMaster, kGet, id, spaceId);
	if (res.IsEmpty())
		throw errors::NotFound404{};

	return res.AsSingleRow<model::CcModeType>(pg::kRowTag);
}

const pg::Query kCreate{
	"INSERT INTO project.cc_mode_type (space_id, project_id, cc_type_id, key, name) "
	"VALUES ($1, $2, $3, $4, $5) "
	"RETURNING id",
	pg::Query::Name{"insert_cc_mode_type"},
};

int64_t CcModeType::Create(const model::CcModeType& item)
{
	auto res = _pg->Execute(ClusterHostType::kMaster, kCreate, item.spaceId, item.projectId, item.ccTypeId, item.key, item.name);
	return res.AsSingleRow<int64_t>();
}

const pg::Query kUpdate {
	"UPDATE project.cc_mode_type SET project_id = $3, cc_type_id = $4, key = $5, name = $6 "
	"WHERE id = $1 AND space_id = $2",
	pg::Query::Name{"update_cc_mode_type"},
};

void CcModeType::Update(const model::CcModeType& item) {
	auto res = _pg->Execute(ClusterHostType::kMaster, kUpdate, item.id, item.spaceId, item.projectId, item.ccTypeId, item.key, item.name);
	if (!res.RowsAffected())
		throw errors::NotFound404();
}

const pg::Query kDelete {
	"DELETE FROM project.cc_mode_type WHERE id = $1 AND space_id = $2",
	pg::Query::Name{"delete_cc_mode_type"},
};

void CcModeType::Delete(const boost::uuids::uuid& spaceId, int64_t id) {
	auto res = _pg->Execute(ClusterHostType::kMaster, kDelete, id, spaceId);
	if (!res.RowsAffected())
		throw errors::NotFound404();
}

const pg::Query kSelectCcModeTypes{
	"SELECT id, space_id, project_id, cc_type_id, key, name FROM project.cc_mode_type "
	"WHERE space_id = $1 AND cc_type_id = $2 "
	"OFFSET $3 LIMIT $4",
	pg::Query::Name{"select_cc_mode_types"},
};

const pg::Query kCount{
	"SELECT COUNT(*) FROM project.cc_mode_type "
	"WHERE space_id = $1 AND cc_type_id = $2",
	pg::Query::Name{"count_cc_mode_types"},
};

PagingResult<model::CcModeType> CcModeType::GetList(const boost::uuids::uuid& spaceId, int64_t ccTypeId, int start, int limit) {
	PagingResult<model::CcModeType> data;

	auto trx = _pg->Begin(pg::Transaction::RO);
	auto res = trx.Execute(kSelectCcModeTypes, spaceId, ccTypeId, start, limit);
	data.items = res.AsContainer<decltype(data.items)>(pg::kRowTag);
	res = trx.Execute(kCount, spaceId, ccTypeId);
	data.total = res.AsSingleRow<int64_t>();
	trx.Commit();
	return data;
}

const pg::Query kGetListByProject{
	"SELECT id, space_id, project_id, cc_type_id, key, name FROM project.cc_mode_type "
	"WHERE space_id = $1 AND project_id = $2 "
	"OFFSET $3 LIMIT $4",
	pg::Query::Name{"select_cc_mode_types_by_project"},
};

const pg::Query kGetListByProjectCount{
	"SELECT COUNT(*) FROM project.cc_mode_type "
	"WHERE space_id = $1 AND project_id = $2",
	pg::Query::Name{"count_cc_mode_types_by_project"},
};

PagingResult<model::CcModeType> CcModeType::GetListByProjectId(
		const boost::uuids::uuid& spaceId,
		const boost::uuids::uuid& projectId,
		int start, int limit)
{
	PagingResult<model::CcModeType> data;

	auto trx = _pg->Begin(pg::Transaction::RO);
	auto res = trx.Execute(kGetListByProject, spaceId, projectId, start, limit);
	data.items = res.AsContainer<decltype(data.items)>(pg::kRowTag);

	res = trx.Execute(kGetListByProjectCount, spaceId, projectId);
	data.total = res.AsSingleRow<int64_t>();

	trx.Commit();
	return data;
}

} // namespace svetit::project::table
