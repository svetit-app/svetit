#pragma once

#include "../model/space_link.hpp"
#include "../../../shared/paging.hpp"

#include <userver/components/loggable_component_base.hpp>
#include <userver/utest/using_namespace_userver.hpp>
#include <userver/storages/postgres/cluster.hpp>

namespace svetit::space::table {

class SpaceLink final {
public:
	explicit SpaceLink(storages::postgres::ClusterPtr pg);
	void Insert(
		const boost::uuids::uuid& spaceId,
		const std::string& creatorId,
		const std::string& name,
		int64_t expiredAt);
	PagingResult<model::SpaceLink> Select(int offset, int limit);
	PagingResult<model::SpaceLink> SelectBySpace(const boost::uuids::uuid& spaceId, int offset, int limit);
	void InsertDataForMocks();
	void DeleteBySpace(const boost::uuids::uuid& spaceUuid);
	void DeleteById(const boost::uuids::uuid& id);
	model::SpaceLink SelectById(const boost::uuids::uuid& id);

private:
	storages::postgres::ClusterPtr _pg;
};

} // namespace svetit::space::table
