#pragma once

#include "../model/node_project.hpp"
#include <shared/paging.hpp>
#include <shared/db/db_base.hpp>

#include <userver/components/loggable_component_base.hpp>
#include <userver/utest/using_namespace_userver.hpp>
#include <userver/storages/postgres/cluster.hpp>

namespace svetit::node::table {

class NodeProject final {
public:
	explicit NodeProject(std::shared_ptr<db::Base> dbPtr);

private:
	std::shared_ptr<db::Base> _db;
};

} // namespace svetit::node::table
