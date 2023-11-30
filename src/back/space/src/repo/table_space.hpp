#pragma once

#include "../model/space.hpp"
#include "../../../shared/paging.hpp"

#include <userver/components/loggable_component_base.hpp>
#include <userver/utest/using_namespace_userver.hpp>
#include <userver/storages/postgres/cluster.hpp>

namespace svetit::space::table {

class Space final {
public:
	explicit Space(storages::postgres::ClusterPtr pg);
	boost::uuids::uuid Insert(const std::string& name, const std::string& key, bool requestsAllowed);
	int CountByUserId(const std::string& userId);
	void InsertDataForMocks();
	bool IsExists(const std::string& key);
	void Delete(const boost::uuids::uuid& spaceUuid);
	model::Space SelectById(const boost::uuids::uuid& id);
	model::Space SelectByKey(const std::string& key);

private:
	storages::postgres::ClusterPtr _pg;
};

} // namespace svetit::space::table
