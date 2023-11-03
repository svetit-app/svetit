#pragma once

#include <optional>
#include <string>
#include <string_view>
#include <chrono>

#include <userver/components/loggable_component_base.hpp>
#include <userver/yaml_config/schema.hpp>
#include <userver/utest/using_namespace_userver.hpp>
#include <userver/storages/postgres/cluster.hpp>

namespace svetit::space::table {

class SpaceUser final {
public:
	explicit SpaceUser(storages::postgres::ClusterPtr pg);
	void Insert(
		const boost::uuids::uuid& spaceId,
		const boost::uuids::uuid& userId,
		const bool& isOwner,
		std::chrono::system_clock::time_point joinedAt,
		const std::string& role
	);
	void InsertDataForMocks();
	bool DeleteBySpace(boost::uuids::uuid spaceUuid);

private:
	storages::postgres::ClusterPtr _pg;
};

} // namespace svetit::space::table
