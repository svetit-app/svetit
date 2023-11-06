#pragma once

#include "../model/space_link.hpp"

#include <optional>
#include <string>
#include <string_view>
#include <chrono>

#include <userver/components/loggable_component_base.hpp>
#include <userver/yaml_config/schema.hpp>
#include <userver/utest/using_namespace_userver.hpp>
#include <userver/storages/postgres/cluster.hpp>

namespace svetit::space::table {

class SpaceLink final {
public:
	explicit SpaceLink(storages::postgres::ClusterPtr pg);
	void Insert(
		const boost::uuids::uuid& id,
		const boost::uuids::uuid& spaceId,
		const boost::uuids::uuid& creatorId,
		const std::string& name,
		std::chrono::system_clock::time_point createdAt,
		std::chrono::system_clock::time_point expiredAt
	);
	std::vector<model::SpaceLink> Select(const int& offset, const int& limit);
	int Count();
	void InsertDataForMocks();
	bool DeleteBySpace(boost::uuids::uuid spaceUuid);
	bool DeleteById(const boost::uuids::uuid id);
	boost::uuids::uuid GetSpaceId(boost::uuids::uuid id);
	model::SpaceLink SelectById(boost::uuids::uuid id, bool& found);

private:
	storages::postgres::ClusterPtr _pg;
};

} // namespace svetit::space::table
