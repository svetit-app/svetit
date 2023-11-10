#pragma once

#include "../model/space.hpp"

#include <optional>
#include <string>
#include <string_view>
#include <chrono>

#include <userver/components/loggable_component_base.hpp>
#include <userver/yaml_config/schema.hpp>
#include <userver/utest/using_namespace_userver.hpp>
#include <userver/storages/postgres/cluster.hpp>

#include <boost/uuid/uuid.hpp>

namespace svetit::space::table {

class Space final {
public:
	explicit Space(storages::postgres::ClusterPtr pg);
	void Insert(
		const boost::uuids::uuid& uuid,
		const std::string& name,
		const std::string& key,
		const bool requestsAllowed,
		int64_t createdAt);
	std::vector<model::Space> Select(const int& offset, const int& limit);
	std::vector<model::Space> SelectAvailable(const std::string userId, const int& offset, const int& limit);
	std::vector<model::Space> SelectByUserId(const std::string userId, const int& offset, const int& limit);
	int Count();
	int CountAvailable(const std::string userId);
	int CountByUserId(const std::string userId);
	void InsertDataForMocks();
	bool IsExists(std::string key);
	bool IsReadyForCreationByTime(std::string userId);
	int GetCountSpacesWithUser(std::string userId);
	bool Delete(boost::uuids::uuid spaceUuid);
	model::Space SelectById(boost::uuids::uuid id);
	model::Space SelectByKey(std::string key);

private:
	storages::postgres::ClusterPtr _pg;
};

} // namespace svetit::space::table
