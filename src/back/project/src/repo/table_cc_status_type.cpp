#include "table_cc_status_type.hpp"
#include <shared/errors.hpp>
#include <shared/paging.hpp>

#include <userver/components/component_config.hpp>
#include <userver/components/component_context.hpp>
#include <userver/yaml_config/merge_schemas.hpp>
#include <userver/storages/postgres/component.hpp>

namespace svetit::project::table {

namespace pg = storages::postgres;
using pg::ClusterHostType;

CcStatusType::CcStatusType(pg::ClusterPtr pg)
	: _pg{std::move(pg)}
{}

const pg::Query kSelect{
	"SELECT id, cc_type_id, category_id, key, text, inform FROM project.cc_status_type WHERE id = $1",
	pg::Query::Name{"select_cc_status_type"},
};

model::CcStatusType CcStatusType::Select(int id) {
	auto res = _pg->Execute(ClusterHostType::kMaster, kSelect, id);
	if (res.IsEmpty())
		throw errors::NotFound404{};

	return res.AsSingleRow<model::CcStatusType>(pg::kRowTag);
}

const pg::Query kInsert{
	"INSERT INTO project.cc_status_type (cc_type_id, category_id, key, text, inform) "
	"VALUES ($1, $2, $3, $4, $5)",
	pg::Query::Name{"insert_cc_status_type"},
};

void CcStatusType::Insert(
		int ccTypeId,
		int categoryId,
		const std::string& key,
		const std::string& text,
		bool inform)
{
	_pg->Execute(ClusterHostType::kMaster, kInsert, ccTypeId, categoryId, key, text, inform);
}

const pg::Query kUpdate {
	"UPDATE project.cc_status_type SET cc_type_id = $2, category_id = $3, key = $4, text = $5, inform = $6 "
	"WHERE id = $1",
	pg::Query::Name{"update_cc_status_type"},
};

void CcStatusType::Update(const model::CcStatusType& ccStatusType) {
	auto res = _pg->Execute(ClusterHostType::kMaster, kUpdate, ccStatusType.id, ccStatusType.ccTypeId, ccStatusType.categoryId, ccStatusType.key, ccStatusType.text, ccStatusType.inform);
	if (!res.RowsAffected())
		throw errors::NotFound404();
}

const pg::Query kDelete {
	"DELETE FROM project.cc_status_type WHERE id = $1",
	pg::Query::Name{"delete_cc_status_type"},
};

void CcStatusType::Delete(int id) {
	auto res = _pg->Execute(ClusterHostType::kMaster, kDelete, id);
	if (!res.RowsAffected())
		throw errors::NotFound404();
}

const pg::Query kSelectStatusTypes{
	"SELECT id, cc_type_id, category_id, key, text, inform FROM project.cc_status_type "
	"OFFSET $1 LIMIT $2",
	pg::Query::Name{"select_cc_status_types"},
};

const pg::Query kCount{
	"SELECT COUNT(*) FROM project.cc_status_type",
	pg::Query::Name{"count_cc_status_types"},
};

PagingResult<model::CcStatusType> CcStatusType::GetList(int start, int limit) {
	PagingResult<model::CcStatusType> data;

	auto trx = _pg->Begin(pg::Transaction::RO);
	auto res = trx.Execute(kSelectStatusTypes, start, limit);
	data.items = res.AsContainer<decltype(data.items)>(pg::kRowTag);
	res = trx.Execute(kCount);
	data.total = res.AsSingleRow<int64_t>();
	trx.Commit();
	return data;
}

} // namespace svetit::project::table