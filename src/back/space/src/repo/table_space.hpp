#pragma once

#include "../model/space.hpp"
#include <shared/paging.hpp>
#include <shared/db/db_base.hpp>

#include <userver/components/loggable_component_base.hpp>
#include <userver/utest/using_namespace_userver.hpp>
#include <userver/storages/postgres/cluster.hpp>

namespace svetit::space::table {

class Space final {
public:
	explicit Space(std::shared_ptr<db::Base> dbPtr);
	boost::uuids::uuid Create(const std::string& name, const std::string& key, bool requestsAllowed);
	int CountByUserId(const std::string& userId);
	void InsertDataForMocks();
	bool IsExists(const std::string& key);
	void Delete(const boost::uuids::uuid& spaceId);
	model::Space SelectById(const boost::uuids::uuid& id);
	model::Space SelectByKey(const std::string& key);

private:
	std::shared_ptr<db::Base> _db;
};

} // namespace svetit::space::table
