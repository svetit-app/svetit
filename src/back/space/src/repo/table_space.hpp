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
		std::chrono::system_clock::time_point createdAt
	);
	std::vector<model::Space> Select(const int& offset, const int& limit);
	std::vector<model::Space> SelectAvailable(const boost::uuids::uuid userId, const int& offset, const int& limit);
	std::vector<model::Space> SelectByUserId(const boost::uuids::uuid userId, const int& offset, const int& limit);
	int Count();
	int CountAvailable(const boost::uuids::uuid userId);
	int CountByUserId(const boost::uuids::uuid userId);
	void InsertDataForMocks();
	bool IsExists(std::string key);
	bool IsReadyForCreationByTime(boost::uuids::uuid userId);
	int GetCountSpacesWithUser(boost::uuids::uuid userUuid);
	bool Delete(boost::uuids::uuid spaceUuid);
	model::Space SelectById(boost::uuids::uuid id, bool& found);
	model::Space SelectByKey(std::string key, bool& found);

private:
	storages::postgres::ClusterPtr _pg;
};

} // namespace svetit::space::table
