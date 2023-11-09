#pragma once

#include "../model/space_invitation.hpp"
#include "../model/errors.hpp"

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
		const std::string& userId,
		const std::string& role,
		const std::string& creatorId,
		int64_t createdAt);
	std::vector<model::SpaceInvitation> Select(const int& offset, const int& limit);
	int Count();
	int GetAvailableCount(const std::string currentUserId);
	void InsertDataForMocks();
	bool DeleteBySpace(boost::uuids::uuid spaceUuid);
	bool UpdateRole(const int id, const std::string role);
	model::SpaceInvitation SelectById(const int id);
	bool DeleteById(const int id);

private:
	storages::postgres::ClusterPtr _pg;
};

} // namespace svetit::space::table
