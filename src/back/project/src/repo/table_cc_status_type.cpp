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

const pg::Query kGet{
	"SELECT id, space_id, cc_type_id, category_id, key, text, inform FROM project.cc_status_type WHERE id = $1 AND space_id = $2",
	pg::Query::Name{"select_cc_status_type"},
};

model::CcStatusType CcStatusType::Get(const boost::uuids::uuid& spaceId, int64_t id) {
	auto res = _pg->Execute(ClusterHostType::kMaster, kGet, id, spaceId);
	if (res.IsEmpty())
		throw errors::NotFound404{};

	return res.AsSingleRow<model::CcStatusType>(pg::kRowTag);
}

const pg::Query kCreate{
	"INSERT INTO project.cc_status_type (space_id, cc_type_id, category_id, key, text, inform) "
	"VALUES ($1, $2, $3, $4, $5, $6)"
	"RETURNING id",
	pg::Query::Name{"insert_cc_status_type"},
};

int64_t CcStatusType::Create(const model::CcStatusType& item)
{
	auto res =_pg->Execute(ClusterHostType::kMaster, kCreate, item.spaceId, item.ccTypeId, item.categoryId, item.key, item.text, item.inform);
	return res.AsSingleRow<int64_t>();
}

const pg::Query kUpdate {
	"UPDATE project.cc_status_type SET cc_type_id = $3, category_id = $4, key = $5, text = $6, inform = $7 "
	"WHERE id = $1 AND space_id = $2",
	pg::Query::Name{"update_cc_status_type"},
};

void CcStatusType::Update(const model::CcStatusType& item) {
	auto res = _pg->Execute(ClusterHostType::kMaster, kUpdate, item.id, item.spaceId, item.ccTypeId, item.categoryId, item.key, item.text, item.inform);
	if (!res.RowsAffected())
		throw errors::NotFound404();
}

const pg::Query kDelete {
	"DELETE FROM project.cc_status_type WHERE id = $1 AND space_id = $2",
	pg::Query::Name{"delete_cc_status_type"},
};

void CcStatusType::Delete(const boost::uuids::uuid& spaceId, int64_t id) {
	auto res = _pg->Execute(ClusterHostType::kMaster, kDelete, id, spaceId);
	if (!res.RowsAffected())
		throw errors::NotFound404();
}

const pg::Query kSelectStatusTypes{
	"SELECT id, space_id, cc_type_id, category_id, key, text, inform FROM project.cc_status_type "
	"WHERE space_id = $1 AND cc_type_id = $2"
	"OFFSET $3 LIMIT $4",
	pg::Query::Name{"select_cc_status_types"},
};

const pg::Query kCount{
	"SELECT COUNT(*) FROM project.cc_status_type "
	"WHERE space_id = $1 AND cc_type_id = $2",
	pg::Query::Name{"count_cc_status_types"},
};

PagingResult<model::CcStatusType> CcStatusType::GetList(const boost::uuids::uuid& spaceId, int64_t ccTypeId, int start, int limit) {
	PagingResult<model::CcStatusType> data;

	auto trx = _pg->Begin(pg::Transaction::RO);
	auto res = trx.Execute(kSelectStatusTypes, spaceId, ccTypeId, start, limit);
	data.items = res.AsContainer<decltype(data.items)>(pg::kRowTag);
	res = trx.Execute(kCount, spaceId, ccTypeId);
	data.total = res.AsSingleRow<int64_t>();
	trx.Commit();
	return data;
}

} // namespace svetit::project::table
