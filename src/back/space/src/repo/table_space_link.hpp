#pragma once

#include "../model/space_link.hpp"
#include <svetit/paging.hpp>
#include <svetit/db/db_base.hpp>

#include <userver/components/loggable_component_base.hpp>
#include <userver/utest/using_namespace_userver.hpp>
#include <userver/storages/postgres/cluster.hpp>

namespace svetit::space::table {

class SpaceLink final {
public:
	explicit SpaceLink(std::shared_ptr<db::Base> dbPtr);
	void Insert(
		const boost::uuids::uuid& spaceId,
		const std::string& creatorId,
		const std::string& name,
		std::chrono::system_clock::time_point expiredAt);
	PagingResult<model::SpaceLink> SelectBySpace(const boost::uuids::uuid& spaceId, int offset, int limit);
	void DeleteBySpace(const boost::uuids::uuid& spaceId);
	void DeleteById(const boost::uuids::uuid& id);
	model::SpaceLink SelectById(const boost::uuids::uuid& id);

private:
	std::shared_ptr<db::Base> _db;
};

} // namespace svetit::space::table
