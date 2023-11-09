#pragma once

#include <optional>
#include <string>
#include <string_view>
#include <chrono>

#include "../model/space_user.hpp"
#include "../model/errors.hpp"

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
		const std::string& userId,
		const bool& isOwner,
		int64_t joinedAt,
		const std::string& role);
	void InsertDataForMocks();
	bool DeleteBySpace(boost::uuids::uuid spaceUuid);
	bool IsOwner(boost::uuids::uuid spaceUuid, std::string userId);
	bool IsUserInside(boost::uuids::uuid spaceUuid, std::string userId);
	model::SpaceUser GetByIds(boost::uuids::uuid spaceUuid, std::string userId);
	bool IsAdmin(boost::uuids::uuid spaceUuid, std::string userId);
	bool Delete(boost::uuids::uuid spaceUuid, std::string userId);
	bool Update(model::SpaceUser user);
	std::vector<model::SpaceUser> Get(boost::uuids::uuid spaceUuid, int start, int limit);
	int CountBySpaceId(const boost::uuids::uuid spaceId);

private:
	storages::postgres::ClusterPtr _pg;
};

} // namespace svetit::space::table
