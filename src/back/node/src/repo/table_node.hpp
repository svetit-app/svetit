#pragma once

#include "../model/node.hpp"
#include <shared/paging.hpp>
#include <shared/db/db_base.hpp>

#include <userver/components/loggable_component_base.hpp>
#include <userver/utest/using_namespace_userver.hpp>
#include <userver/storages/postgres/cluster.hpp>

namespace svetit::node::table {

class Node final {
public:
	explicit Node(std::shared_ptr<db::Base> dbPtr);
	model::Node Select(const boost::uuids::uuid& id);
	boost::uuids::uuid Create(const model::Node& item);
	void Delete(const boost::uuids::uuid& id);
	void Update(const model::Node& item);
	PagingResult<model::Node> SelectList(int32_t start, int32_t limit);

private:
	std::shared_ptr<db::Base> _db;
};

} // namespace svetit::node::table
