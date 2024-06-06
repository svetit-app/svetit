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
	"SELECT id, space_id, project_id, parent_id, key, name, description, value_type FROM project.param_type WHERE id = $1 AND space_id = $2",
	pg::Query::Name{"select_param_type"}
};

model::ParamType ParamType::Get(const boost::uuids::uuid& spaceId, int64_t id) {
	auto res = _pg->Execute(ClusterHostType::kMaster, kGet, id, spaceId);
	if (res.IsEmpty())
		throw errors::NotFound404{};

	return res.AsSingleRow<model::ParamType>(pg::kRowTag);
}

const pg::Query kCreate{
	"INSERT INTO project.param_type (space_id, project_id, parent_id, key, name, description, value_type) "
	"VALUES ($1, $2, $3, $4, $5, $6, $7) "
	"RETURNING id",
	pg::Query::Name{"insert_param_type"},
};

const pg::Query kCreateWithNulledParentId{
	"INSERT INTO project.param_type (space_id, project_id, parent_id, key, name, description, value_type) "
	"VALUES ($1, $2, NULL, $3, $4, $5, $6) "
	"RETURNING id",
	pg::Query::Name{"insert_param_type"},
};

int64_t ParamType::Create(const model::ParamType& item)
{
	if (item.parentId.has_value())
	{
		auto res = _pg->Execute(ClusterHostType::kMaster, kCreate, item.spaceId, item.projectId, item.parentId.value(), item.key, item.name, item.description, item.valueType);
		return res.AsSingleRow<int64_t>();
	}

	auto res = _pg->Execute(ClusterHostType::kMaster, kCreateWithNulledParentId, item.spaceId, item.projectId, item.key, item.name, item.description, item.valueType);
	return res.AsSingleRow<int64_t>();
}

const pg::Query kUpdate {
	"UPDATE project.param_type SET project_id = $3, parent_id = $4, key = $5, name = $6, description = $7, value_type = $8 "
	"WHERE id = $1 AND space_id = $2",
	pg::Query::Name{"update_param_type"},
};

void ParamType::Update(const model::ParamType& paramType) {
	auto res = _pg->Execute(ClusterHostType::kMaster, kUpdate, paramType.id, paramType.spaceId, paramType.projectId, paramType.parentId, paramType.key, paramType.name, paramType.description, paramType.valueType);
	if (!res.RowsAffected())
		throw errors::NotFound404();
}

const pg::Query kDelete {
	"DELETE FROM project.param_type WHERE id = $1 AND space_id = $2",
	pg::Query::Name{"delete_param_type"},
};

void ParamType::Delete(const boost::uuids::uuid& spaceId, int64_t id) {
	auto res = _pg->Execute(ClusterHostType::kMaster, kDelete, id, spaceId);
	if (!res.RowsAffected())
		throw errors::NotFound404();
}

const pg::Query kSelectParamTypes{
	"SELECT id, space_id, project_id, parent_id, key, name, description, value_type FROM project.param_type "
	"WHERE space_id = $1 AND project_id = $2 "
	"OFFSET $3 LIMIT $4",
	pg::Query::Name{"select_param_types"},
};

const pg::Query kCount{
	"SELECT COUNT(*) FROM project.param_type "
	"WHERE space_id = $1 AND project_id = $2",
	pg::Query::Name{"count_param_types"},
};

PagingResult<model::ParamType> ParamType::GetList(const boost::uuids::uuid& spaceId, const boost::uuids::uuid& projectId, int start, int limit) {
	PagingResult<model::ParamType> data;

	auto trx = _pg->Begin(pg::Transaction::RO);
	auto res = trx.Execute(kSelectParamTypes, spaceId, projectId, start, limit);
	data.items = res.AsContainer<decltype(data.items)>(pg::kRowTag);
	res = trx.Execute(kCount, spaceId, projectId);
	data.total = res.AsSingleRow<int64_t>();
	trx.Commit();
	return data;
}

} // namespace svetit::project::table
