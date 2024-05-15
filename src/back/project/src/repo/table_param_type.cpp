#include "table_param_type.hpp"
#include <shared/errors.hpp>
#include <shared/paging.hpp>

#include <userver/components/component_config.hpp>
#include <userver/components/component_context.hpp>
#include <userver/yaml_config/merge_schemas.hpp>
#include <userver/storages/postgres/component.hpp>

namespace svetit::project::table {

namespace pg = storages::postgres;
using pg::ClusterHostType;

ParamType::ParamType(pg::ClusterPtr pg)
	: _pg{std::move(pg)}
{}

const pg::Query kGet{
	"SELECT id, parent_id, key, name, description, value_type FROM project.param_type WHERE id = $1",
	pg::Query::Name{"select_param_type"}
};

model::ParamType ParamType::Get(int64_t id) {
	auto res = _pg->Execute(ClusterHostType::kMaster, kGet, id);
	if (res.IsEmpty())
		throw errors::NotFound404{};

	return res.AsSingleRow<model::ParamType>(pg::kRowTag);
}

const pg::Query kCreate{
	"INSERT INTO project.param_type (parent_id, key, name, description, value_type) "
	"VALUES ($1, $2, $3, $4, $5)"
	"RETURNING id",
	pg::Query::Name{"insert_param_type"},
};

const pg::Query kCreateWithNulledParentId{
	"INSERT INTO project.param_type (parent_id, key, name, description, value_type) "
	"VALUES (NULL, $1, $2, $3, $4)"
	"RETURNING id",
	pg::Query::Name{"insert_param_type"},
};

int64_t ParamType::Create(const model::ParamType& item)
{
	if (item.parentId.has_value())
	{
		auto res = _pg->Execute(ClusterHostType::kMaster, kCreate, item.parentId.value(), item.key, item.name, item.description, item.valueType);
		return res.AsSingleRow<int64_t>();
	}

	auto res = _pg->Execute(ClusterHostType::kMaster, kCreateWithNulledParentId, item.key, item.name, item.description, item.valueType);
	return res.AsSingleRow<int64_t>();
}

const pg::Query kUpdate {
	"UPDATE project.param_type SET parent_id = $2, key = $3, name = $4, description = $5, value_type = $6 "
	"WHERE id = $1",
	pg::Query::Name{"update_param_type"},
};

void ParamType::Update(const model::ParamType& paramType) {
	auto res = _pg->Execute(ClusterHostType::kMaster, kUpdate, paramType.id, paramType.parentId, paramType.key, paramType.name, paramType.description, paramType.valueType);
	if (!res.RowsAffected())
		throw errors::NotFound404();
}

const pg::Query kDelete {
	"DELETE FROM project.param_type WHERE id = $1",
	pg::Query::Name{"delete_param_type"},
};

void ParamType::Delete(int64_t id) {
	auto res = _pg->Execute(ClusterHostType::kMaster, kDelete, id);
	if (!res.RowsAffected())
		throw errors::NotFound404();
}

const pg::Query kSelectParamTypes{
	"SELECT id, parent_id, key, name, description, value_type FROM project.param_type "
	"OFFSET $1 LIMIT $2",
	pg::Query::Name{"select_param_types"},
};

const pg::Query kCount{
	"SELECT COUNT(*) FROM project.param_type",
	pg::Query::Name{"count_param_types"},
};

PagingResult<model::ParamType> ParamType::GetList(int start, int limit) {
	PagingResult<model::ParamType> data;

	auto trx = _pg->Begin(pg::Transaction::RO);
	auto res = trx.Execute(kSelectParamTypes, start, limit);
	data.items = res.AsContainer<decltype(data.items)>(pg::kRowTag);
	res = trx.Execute(kCount);
	data.total = res.AsSingleRow<int64_t>();
	trx.Commit();
	return data;
}

} // namespace svetit::project::table
