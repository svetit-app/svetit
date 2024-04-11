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
	model::DiType Select(int id);
	int Insert(
		int measureId,
		int saveTimerId,
		const std::string& key,
		const std::string& name,
		DiMode::Type mode,
		SaveAlgorithm::Type saveAlgorithm,
		bool isDeleted);
	void Update(const model::DiType& diType);
	void Delete(int id);
	PagingResult<model::DiType> GetList(int start, int limit);
private:
	storages::postgres::ClusterPtr _pg;
};

} // namespace svetit::project::table