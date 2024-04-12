#pragma once

#include "../model/cc_status_category.hpp"
#include <shared/paging.hpp>

#include <userver/components/loggable_component_base.hpp>
#include <userver/utest/using_namespace_userver.hpp>
#include <userver/storages/postgres/cluster.hpp>

namespace svetit::project::table {

class CcStatusCategory final {
public:
	explicit CcStatusCategory(storages::postgres::ClusterPtr pg);
	model::CcStatusCategory Select(int id);
	int Insert(
		const boost::uuids::uuid& projectId,
		const std::string& key,
		const std::string& name,
		const std::string& color);
	void Update(const model::CcStatusCategory& ccStatusCategory);
	void Delete(int id);
	PagingResult<model::CcStatusCategory> GetList(int start, int limit);
private:
	storages::postgres::ClusterPtr _pg;
};

} // namespace svetit::project::table
