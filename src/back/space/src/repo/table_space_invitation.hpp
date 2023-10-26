#pragma once

#include "../model/space_invitation.hpp"

#include <optional>
#include <string>
#include <string_view>
#include <chrono>

#include <userver/components/loggable_component_base.hpp>
#include <userver/yaml_config/schema.hpp>
#include <userver/utest/using_namespace_userver.hpp>
#include <userver/storages/postgres/cluster.hpp>

namespace svetit::space::table {

class SpaceInvitation final {
public:
	explicit SpaceInvitation(storages::postgres::ClusterPtr pg);
	void Insert(
		const boost::uuids::uuid& spaceId,
		const boost::uuids::uuid& userId,
		const std::string& role,
		const boost::uuids::uuid& creatorId,
		std::chrono::system_clock::time_point createdAt
	);
	std::vector<model::SpaceInvitation> Select(const int& offset, const int& limit);
	int Count();
	void InsertDataForMocks();

private:
	storages::postgres::ClusterPtr _pg;
};

} // namespace svetit::space::table
