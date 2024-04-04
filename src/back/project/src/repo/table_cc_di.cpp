#include "table_cc_di.hpp"
#include <shared/errors.hpp>
#include <shared/paging.hpp>

#include <userver/components/component_config.hpp>
#include <userver/components/component_context.hpp>
#include <userver/yaml_config/merge_schemas.hpp>
#include <userver/storages/postgres/component.hpp>

namespace svetit::project::table {

namespace pg = storages::postgres;
using pg::ClusterHostType;

CcDi::CcDi(pg::ClusterPtr pg)
	: _pg{std::move(pg)}
{}

const pg::Query kSelect{
	"SELECT cc_id, di_id, is_deleted FROM project.cc_di WHERE cc_id = $1 AND di_id = $2",
	pg::Query::Name{"select_cc_di"},
};

model::CcDi CcDi::Select(int ccId, int diId) {
	auto res = _pg->Execute(ClusterHostType::kMaster, kSelect, ccId, diId);
	if (res.IsEmpty())
		throw errors::NotFound404{};

	return res.AsSingleRow<model::CcDi>(pg::kRowTag);
}

const pg::Query kInsert{
	"INSERT INTO project.cc_di (cc_id, di_id, is_deleted) "
	"VALUES ($1, $2, $3) RETURNING cc_id, di_id",
	pg::Query::Name{"insert_cc_di"},
};

void CcDi::Insert(
		int ccId,
		int diId,
		bool isDeleted)
{
	const auto res =_pg->Execute(ClusterHostType::kMaster, kInsert, ccId, diId, isDeleted);
	// is needed to return vector or pair with inserted row primary key?
}

const pg::Query kUpdate {
	"UPDATE project.cc_di SET is_deleted = $3 "
	"WHERE cc_id = $1 AND di_id = $2",
	pg::Query::Name{"update_cc_di"},
};

void CcDi::Update(const model::CcDi& ccDi) {
	auto res = _pg->Execute(ClusterHostType::kMaster, kUpdate, ccDi.ccId, ccDi.diId, ccDi.isDeleted);
	if (!res.RowsAffected())
		throw errors::NotFound404();
}

const pg::Query kDelete {
	"DELETE FROM project.cc_di WHERE cc_id = $1 AND di_id = $2",
	pg::Query::Name{"delete_cc_di"},
};

void CcDi::Delete(int ccId, int diId) {
	auto res = _pg->Execute(ClusterHostType::kMaster, kDelete, ccId, diId);
	if (!res.RowsAffected())
		throw errors::NotFound404();
}

const pg::Query kSelectCcDis{
	"SELECT cc_id, di_id, is_deleted FROM project.cc_di "
	"OFFSET $1 LIMIT $2",
	pg::Query::Name{"select_cc_dis"},
};

const pg::Query kCount{
	"SELECT COUNT(*) FROM project.cc_di",
	pg::Query::Name{"count_cc_dis"},
};

PagingResult<model::CcDi> CcDi::GetList(int start, int limit) {
	PagingResult<model::CcDi> data;

	auto trx = _pg->Begin(pg::Transaction::RO);
	auto res = trx.Execute(kSelectCcDis, start, limit);
	data.items = res.AsContainer<decltype(data.items)>(pg::kRowTag);
	res = trx.Execute(kCount);
	data.total = res.AsSingleRow<int64_t>();
	trx.Commit();
	return data;
}

} // namespace svetit::project::table