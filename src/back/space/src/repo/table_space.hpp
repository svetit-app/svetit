#pragma once

#include "../model/space.hpp"

#include <userver/components/loggable_component_base.hpp>
#include <userver/utest/using_namespace_userver.hpp>
#include <userver/storages/postgres/cluster.hpp>

namespace svetit::space::table {

class Space final {
public:
	explicit Space(storages::postgres::ClusterPtr pg);
	void Insert(
		const boost::uuids::uuid& uuid,
		const std::string& name,
		const std::string& key,
		const bool requestsAllowed,
		const int64_t createdAt);
	std::vector<model::Space> SelectAvailable(const std::string& userId, const int offset, const int limit);
	std::vector<model::Space> SelectByUserId(const std::string& userId, const int offset, const int limit);
	int CountAvailable(const std::string& userId);
	int CountByUserId(const std::string& userId);
	void InsertDataForMocks();
	bool IsExists(const std::string& key);
	bool IsReadyForCreationByTime(const std::string& userId);
	int GetCountSpacesWithUser(const std::string& userId);
	void Delete(const boost::uuids::uuid& spaceUuid);
	model::Space SelectById(const boost::uuids::uuid& id);
	model::Space SelectByKey(const std::string& key);

private:
	storages::postgres::ClusterPtr _pg;
};

} // namespace svetit::space::table
