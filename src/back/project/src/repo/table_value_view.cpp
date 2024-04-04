#include "table_value_view.hpp"
#include <shared/errors.hpp>
#include <shared/paging.hpp>

#include <userver/components/component_config.hpp>
#include <userver/components/component_context.hpp>
#include <userver/utils/boost_uuid4.hpp>
#include <userver/yaml_config/merge_schemas.hpp>
#include <userver/storages/postgres/component.hpp>

#include <boost/uuid/uuid_io.hpp>

namespace svetit::project::table {

namespace pg = storages::postgres;
using pg::ClusterHostType;

ValueView::ValueView(pg::ClusterPtr pg)
	: _pg{std::move(pg)}
{}

const pg::Query kSelect{
	"SELECT id, di_type_id, value, view, is_deleted FROM project.value_view WHERE id = $1",
	pg::Query::Name{"select_value_view"},
};

model::ValueView ValueView::Select(int id) {
	auto res = _pg->Execute(ClusterHostType::kMaster, kSelect, id);
	if (res.IsEmpty())
		throw errors::NotFound404{};

	return res.AsSingleRow<model::ValueView>(pg::kRowTag);
}

const pg::Query kInsert{
	"INSERT INTO project.value_view (di_type_id, value, view, is_deleted) "
	"VALUES ($1, $2, $3, $4) RETURNING id",
	pg::Query::Name{"insert_value_view"},
};

int ValueView::Insert(
		int diTypeId,
		const std::string& value,
		const std::string& view,
		bool isDeleted)
{
	const auto res =_pg->Execute(ClusterHostType::kMaster, kInsert, diTypeId, value, view, isDeleted);
	return res.AsSingleRow<int>();
}

const pg::Query kUpdate {
	"UPDATE project.value_view SET di_type_id = $2, value = $3, view = $4, is_deleted = $5 "
	"WHERE id = $1",
	pg::Query::Name{"update_value_view"},
};

void ValueView::Update(const model::ValueView& valueView) {
	auto res = _pg->Execute(ClusterHostType::kMaster, kUpdate, valueView.id, valueView.diTypeId, valueView.value, valueView.view, valueView.isDeleted);
	if (!res.RowsAffected())
		throw errors::NotFound404();
}

const pg::Query kDelete {
	"DELETE FROM project.value_view WHERE id = $1",
	pg::Query::Name{"delete_value_view"},
};

void ValueView::Delete(int id) {
	auto res = _pg->Execute(ClusterHostType::kMaster, kDelete, id);
	if (!res.RowsAffected())
		throw errors::NotFound404();
}

const pg::Query kSelectValueViews{
	"SELECT id, di_type_id, value, view, is_deleted FROM project.value_view "
	"OFFSET $1 LIMIT $2",
	pg::Query::Name{"select_value_views"},
};

const pg::Query kCount{
	"SELECT COUNT(*) FROM project.value_view",
	pg::Query::Name{"count_value_views"},
};

PagingResult<model::ValueView> ValueView::GetList(int start, int limit) {
	PagingResult<model::ValueView> data;

	auto trx = _pg->Begin(pg::Transaction::RO);
	auto res = trx.Execute(kSelectValueViews, start, limit);
	data.items = res.AsContainer<decltype(data.items)>(pg::kRowTag);
	res = trx.Execute(kCount);
	data.total = res.AsSingleRow<int64_t>();
	trx.Commit();
	return data;
}

} // namespace svetit::project::table