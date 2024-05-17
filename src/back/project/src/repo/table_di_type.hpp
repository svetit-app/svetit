#pragma once

#include "../model/di_type.hpp"
#include <shared/paging.hpp>

#include <userver/components/loggable_component_base.hpp>
#include <userver/utest/using_namespace_userver.hpp>
#include <userver/storages/postgres/cluster.hpp>

namespace svetit::project::table {

class DiType final {
public:
	explicit DiType(storages::postgres::ClusterPtr pg);
	model::DiType Get(int64_t id);
	void Insert(
		int64_t measureId,
		int64_t saveTimerId,
		const std::string& key,
		const std::string& name,
		DiMode::Type mode,
		SaveAlgorithm::Type saveAlgorithm);
	void Update(const model::DiType& diType);
	void Delete(int64_t id);
	PagingResult<model::DiType> GetList(int start, int limit);
private:
	storages::postgres::ClusterPtr _pg;
};

} // namespace svetit::project::table
