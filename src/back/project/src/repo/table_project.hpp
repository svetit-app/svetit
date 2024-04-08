#pragma once

#include "../model/project.hpp"
#include "../model/sync_direction.hpp"
#include <shared/paging.hpp>

#include <userver/components/loggable_component_base.hpp>
#include <userver/utest/using_namespace_userver.hpp>
#include <userver/storages/postgres/cluster.hpp>

namespace svetit::project::table {

class Project final {
public:
	explicit Project(storages::postgres::ClusterPtr pg);
	model::Project SelectById(const boost::uuids::uuid& id);
	model::Project SelectByKey(const std::string& key);
	boost::uuids::uuid Insert(
		const boost::uuids::uuid& spaceId,
		const std::string& key,
		const std::string& name,
		const std::string& description,
		std::chrono::system_clock::time_point changedAt,
		SyncDirection::Type sync);
	void Update(const model::Project& project);
	void Delete(const boost::uuids::uuid& id);
	PagingResult<model::Project> GetList(int start, int limit);
private:
	storages::postgres::ClusterPtr _pg;
};

} // namespace svetit::project::table
