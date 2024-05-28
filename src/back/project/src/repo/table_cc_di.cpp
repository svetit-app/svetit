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

const pg::Query kGet{
	"SELECT cc_id, di_id, space_id FROM project.cc_di WHERE space_id = $1 AND cc_id = $2 AND di_id = $3",
	pg::Query::Name{"select_cc_di"},
};

model::CcDi CcDi::Get(const boost::uuids::uuid& spaceId, int64_t ccId, int64_t diId) {
	auto res = _pg->Execute(ClusterHostType::kMaster, kGet, spaceId, ccId, diId);
	if (res.IsEmpty())
		throw errors::NotFound404{};

	return res.AsSingleRow<model::CcDi>(pg::kRowTag);
}

const pg::Query kCreate{
	"INSERT INTO project.cc_di (space_id, cc_id, di_id) "
	"VALUES ($1, $2, $3)",
	pg::Query::Name{"insert_cc_di"},
};

void CcDi::Create(const model::CcDi& item) {
	_pg->Execute(ClusterHostType::kMaster, kCreate, item.spaceId, item.ccId, item.diId);
}

void CcDi::Update(const model::CcDi&) {
	throw errors::Forbidden403();
}

const pg::Query kDelete {
	"DELETE FROM project.cc_di WHERE space_id = $1 AND cc_id = $2 AND di_id = $3",
	pg::Query::Name{"delete_cc_di"},
};

void CcDi::Delete(const boost::uuids::uuid& spaceId, int64_t ccId, int64_t diId) {
	auto res = _pg->Execute(ClusterHostType::kMaster, kDelete, spaceId, ccId, diId);
	if (!res.RowsAffected())
		throw errors::NotFound404();
}

const pg::Query kSelectCcDis{
	"SELECT cc_id, di_id, space_id FROM project.cc_di "
	"WHERE space_id = $1 AND cc_id = $2 "
	"OFFSET $3 LIMIT $4",
	pg::Query::Name{"select_cc_dis"},
};

const pg::Query kCount{
	"SELECT COUNT(*) FROM project.cc_di "
	"WHERE space_id = $1 AND cc_id = $2",
	pg::Query::Name{"count_cc_dis"},
};

PagingResult<model::CcDi> CcDi::GetList(const boost::uuids::uuid& spaceId, int64_t ccId, int start, int limit) {
	PagingResult<model::CcDi> data;

	auto trx = _pg->Begin(pg::Transaction::RO);
	auto res = trx.Execute(kSelectCcDis, spaceId, ccId, start, limit);
	data.items = res.AsContainer<decltype(data.items)>(pg::kRowTag);
	res = trx.Execute(kCount, spaceId, ccId);
	data.total = res.AsSingleRow<int64_t>();
	trx.Commit();
	return data;
}

} // namespace svetit::project::table
