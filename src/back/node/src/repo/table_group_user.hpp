#pragma once

#include "../model/group_user.hpp"
#include <shared/paging.hpp>
#include <shared/db/db_base.hpp>

#include <userver/components/loggable_component_base.hpp>
#include <userver/utest/using_namespace_userver.hpp>
#include <userver/storages/postgres/cluster.hpp>

namespace svetit::node::table {

class GroupUser final {
public:
	explicit GroupUser(std::shared_ptr<db::Base> dbPtr);
	model::GroupUser Select(int groupId, const std::string& userId);
	void Delete(int groupId, const std::string& userId);
	void Create(const model::GroupUser& item);
	PagingResult<model::GroupUser> SelectList(int32_t start, int32_t limit);
	PagingResult<model::GroupUser> SelectListByGroupId(int32_t start, int32_t limit, int groupId);

private:
	std::shared_ptr<db::Base> _db;
};

} // namespace svetit::node::table
