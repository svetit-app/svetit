#pragma once

#include "../model/save_timer.hpp"
#include <shared/paging.hpp>

#include <userver/components/loggable_component_base.hpp>
#include <userver/utest/using_namespace_userver.hpp>
#include <userver/storages/postgres/cluster.hpp>

namespace svetit::project::table {

class SaveTimer final {
public:
	explicit SaveTimer(storages::postgres::ClusterPtr pg);
	model::SaveTimer Get(int64_t id);
	int64_t Create(const model::SaveTimer& saveTimer);
	void Update(const model::SaveTimer& saveTimer);
	void Delete(int64_t id);
	PagingResult<model::SaveTimer> GetList(int start, int limit);
private:
	storages::postgres::ClusterPtr _pg;
};

} // namespace svetit::project::table
