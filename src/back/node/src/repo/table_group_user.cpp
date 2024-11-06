#include "table_group_user.hpp"
#include <memory>
#include <svetit/errors.hpp>
#include <svetit/paging.hpp>

#include <userver/components/component_config.hpp>
#include <userver/components/component_context.hpp>
#include <userver/utils/boost_uuid4.hpp>
#include <userver/yaml_config/merge_schemas.hpp>
#include <userver/storages/postgres/component.hpp>

namespace svetit::node::table {

namespace pg = storages::postgres;
using pg::ClusterHostType;

GroupUser::GroupUser(std::shared_ptr<db::Base> dbPtr)
	: _db{std::move(dbPtr)}
{}

const pg::Query kSelect {
	"SELECT group_id, user_id FROM node.group_user WHERE group_id = $1 AND user_id = $2",
	pg::Query::Name{"groupUser.select"},
};

model::GroupUser GroupUser::Select(int groupId, const std::string& userId) {
	auto res = _db->Execute(ClusterHostType::kSlave, kSelect, groupId, userId);
	if (res.IsEmpty())
		throw errors::NotFound404{};

	return res.AsSingleRow<model::GroupUser>(pg::kRowTag);
}

const pg::Query kDelete {
	"DELETE FROM node.group_user WHERE group_id = $1 AND user_id = $2",
	pg::Query::Name{"groupUser.delete"},
};

void GroupUser::Delete(int groupId, const std::string& userId) {
	auto res = _db->Execute(ClusterHostType::kMaster, kDelete, groupId, userId);
	if (!res.RowsAffected())
		throw errors::NotFound404();
}

const pg::Query kInsert {
	"INSERT INTO node.group_user (group_id, user_id) VALUES ($1, $2)",
	pg::Query::Name{"groupUser.insert"},
};

void GroupUser::Create(const model::GroupUser& item) {
	_db->Execute(ClusterHostType::kMaster, kInsert, item.groupId, item.userId);
}

const pg::Query kSelectList {
	"SELECT group_id, user_id, COUNT(*) OVER() FROM node.group_user OFFSET $1 LIMIT $2",
	pg::Query::Name{"groupUser.select_list"},
};

PagingResult<model::GroupUser> GroupUser::SelectList(int32_t start, int32_t limit) {
	auto res = _db->Execute(ClusterHostType::kSlave, kSelectList, start, limit);
	PagingResult<model::GroupUser> data;
	data = res.AsContainer<decltype(data)::RawContainer>(pg::kRowTag);
	return data;
}

const pg::Query kSelectListByGroupId {
	"SELECT group_id, user_id, COUNT(*) OVER() FROM node.group_user WHERE group_id = $1 OFFSET $2 LIMIT $3",
	pg::Query::Name{"groupUser.select_list_by_groupId"},
};

PagingResult<model::GroupUser> GroupUser::SelectListByGroupId(int32_t start, int32_t limit, int groupId) {
	auto res = _db->Execute(ClusterHostType::kSlave, kSelectListByGroupId, groupId, start, limit);
	PagingResult<model::GroupUser> data;
	data = res.AsContainer<decltype(data)::RawContainer>(pg::kRowTag);
	return data;
}

} // namespace svetit::node::table
