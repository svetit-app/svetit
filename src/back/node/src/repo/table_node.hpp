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
	model::Node Select(const boost::uuids::uuid& id, const boost::uuids::uuid& spaceId);
	boost::uuids::uuid Create(const model::Node& item, const boost::uuids::uuid& spaceId);
	void Delete(const boost::uuids::uuid& id, const boost::uuids::uuid& spaceId);
	void Update(const model::Node& item, const boost::uuids::uuid& spaceId);
	PagingResult<model::Node> SelectList(const boost::uuids::uuid& spaceId, int32_t start, int32_t limit);

private:
	std::shared_ptr<db::Base> _db;
};

} // namespace svetit::node::table
