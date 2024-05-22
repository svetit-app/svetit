#include "table_value_view.hpp"
#include <shared/errors.hpp>
#include <shared/paging.hpp>

#include <userver/components/component_config.hpp>
#include <userver/components/component_context.hpp>
#include <userver/yaml_config/merge_schemas.hpp>
#include <userver/storages/postgres/component.hpp>

namespace svetit::project::table {

namespace pg = storages::postgres;
using pg::ClusterHostType;

ValueView::ValueView(pg::ClusterPtr pg)
	: _pg{std::move(pg)}
{}

const pg::Query kGet{
	"SELECT id, space_id, di_type_id, value, view FROM project.value_view WHERE id = $1 AND space_id = $2",
	pg::Query::Name{"select_value_view"},
};

model::ValueView ValueView::Get(const boost::uuids::uuid& spaceId, int64_t id) {
	auto res = _pg->Execute(ClusterHostType::kMaster, kGet, id, spaceId);
	if (res.IsEmpty())
		throw errors::NotFound404{};

	return res.AsSingleRow<model::ValueView>(pg::kRowTag);
}

const pg::Query kCreate{
	"INSERT INTO project.value_view (space_id, di_type_id, value, view) "
	"VALUES ($1, $2, $3, $4)"
	"RETURNING id",
	pg::Query::Name{"insert_value_view"},
};

int64_t ValueView::Create(const model::ValueView& valueView)
{
	auto res = _pg->Execute(ClusterHostType::kMaster, kCreate, valueView.spaceId, valueView.diTypeId, valueView.value, valueView.view);
	return res.AsSingleRow<int64_t>();
}

const pg::Query kUpdate {
	"UPDATE project.value_view SET di_type_id = $3, value = $4, view = $5 "
	"WHERE id = $1 AND space_id = $2",
	pg::Query::Name{"update_value_view"},
};

void ValueView::Update(const model::ValueView& valueView) {
	auto res = _pg->Execute(ClusterHostType::kMaster, kUpdate, valueView.id, valueView.spaceId, valueView.diTypeId, valueView.value, valueView.view);
	if (!res.RowsAffected())
		throw errors::NotFound404();
}

const pg::Query kDelete {
	"DELETE FROM project.value_view WHERE id = $1 AND space_id = $2",
	pg::Query::Name{"delete_value_view"},
};

void ValueView::Delete(const boost::uuids::uuid& spaceId, int64_t id) {
	auto res = _pg->Execute(ClusterHostType::kMaster, kDelete, id, spaceId);
	if (!res.RowsAffected())
		throw errors::NotFound404();
}

const pg::Query kSelectValueViews{
	"SELECT id, space_id, di_type_id, value, view FROM project.value_view "
	"WHERE space_id = $1"
	"OFFSET $2 LIMIT $3",
	pg::Query::Name{"select_value_views"},
};

const pg::Query kCount{
	"SELECT COUNT(*) FROM project.value_view",
	pg::Query::Name{"count_value_views"},
};

PagingResult<model::ValueView> ValueView::GetList(const boost::uuids::uuid& spaceId, int start, int limit) {
	PagingResult<model::ValueView> data;

	auto trx = _pg->Begin(pg::Transaction::RO);
	auto res = trx.Execute(kSelectValueViews, spaceId, start, limit);
	data.items = res.AsContainer<decltype(data.items)>(pg::kRowTag);
	res = trx.Execute(kCount);
	data.total = res.AsSingleRow<int64_t>();
	trx.Commit();
	return data;
}

} // namespace svetit::project::table
