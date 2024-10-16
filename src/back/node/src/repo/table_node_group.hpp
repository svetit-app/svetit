#pragma once

#include "../model/node_group.hpp"
#include <svetit/paging.hpp>
#include <svetit/db/db_base.hpp>

#include <userver/components/loggable_component_base.hpp>
#include <userver/utest/using_namespace_userver.hpp>
#include <userver/storages/postgres/cluster.hpp>

namespace svetit::node::table {

class NodeGroup final {
public:
	explicit NodeGroup(std::shared_ptr<db::Base> dbPtr);
	model::NodeGroup Select(const boost::uuids::uuid& nodeId, int groupId);
	void Delete(const boost::uuids::uuid& nodeId, int groupId);
	void Create(const model::NodeGroup& item);
	PagingResult<model::NodeGroup> SelectList(int32_t start, int32_t limit);
	PagingResult<model::NodeGroup> SelectListByNodeId(int32_t start, int32_t limit, const boost::uuids::uuid& nodeId);

private:
	std::shared_ptr<db::Base> _db;
};

} // namespace svetit::node::table
