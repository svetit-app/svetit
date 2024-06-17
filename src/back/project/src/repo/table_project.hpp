#pragma once

#include "../model/project.hpp"
#include "../model/sync_direction.hpp"
#include <shared/paging.hpp>
#include <shared/db/db_table.hpp>

#include <userver/components/loggable_component_base.hpp>
#include <userver/utest/using_namespace_userver.hpp>
#include <userver/storages/postgres/cluster.hpp>

namespace svetit::project::table {

class Project final : public db::Table<model::Project> {
public:
	explicit Project(storages::postgres::ClusterPtr pg);
	model::Project Get(const boost::uuids::uuid& spaceId, const boost::uuids::uuid& id);
	model::Project GetByKey(const boost::uuids::uuid& spaceId, const std::string& key);
	boost::uuids::uuid Create(const model::Project& item);
	void Update(const model::Project& item);
	void Delete(const boost::uuids::uuid& spaceId, const boost::uuids::uuid& id);
	PagingResult<model::Project> GetList(const boost::uuids::uuid& spaceId, int start, int limit);
private:
	storages::postgres::ClusterPtr _pg;
};

} // namespace svetit::project::table
