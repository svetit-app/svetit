#pragma once

#include "../model/translation.hpp"
#include <shared/paging.hpp>

#include <userver/components/loggable_component_base.hpp>
#include <userver/utest/using_namespace_userver.hpp>
#include <userver/storages/postgres/cluster.hpp>

namespace svetit::project::table {

class Translation final {
public:
	explicit Translation(storages::postgres::ClusterPtr pg);
	model::Translation Get(int64_t id);
	void Insert(
		const boost::uuids::uuid& projectId,
		const std::string& lang,
		const std::string& key,
		const std::string& value);
	void Update(const model::Translation& translation);
	void Delete(int64_t id);
	PagingResult<model::Translation> GetList(int start, int limit);
private:
	storages::postgres::ClusterPtr _pg;
};

} // namespace svetit::project::table
