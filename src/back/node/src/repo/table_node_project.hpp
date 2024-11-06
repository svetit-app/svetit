#pragma once

#include "../model/node_project.hpp"
#include <svetit/paging.hpp>
#include <svetit/db/db_base.hpp>

#include <userver/components/loggable_component_base.hpp>
#include <userver/utest/using_namespace_userver.hpp>
#include <userver/storages/postgres/cluster.hpp>

namespace svetit::node::table {

class NodeProject final {
public:
	explicit NodeProject(std::shared_ptr<db::Base> dbPtr);
	model::NodeProject Select(const boost::uuids::uuid& nodeId, const boost::uuids::uuid& projectId);
	void Delete(const boost::uuids::uuid& nodeId, const boost::uuids::uuid& projectId);
	void Create(const model::NodeProject& item);
	PagingResult<model::NodeProject> SelectList(int32_t start, int32_t limit);
	PagingResult<model::NodeProject> SelectListByNodeId(int32_t start, int32_t limit, const boost::uuids::uuid& nodeId);

private:
	std::shared_ptr<db::Base> _db;
};

} // namespace svetit::node::table
