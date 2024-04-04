#pragma once

#include "../model/cc_mode_type.hpp"
#include <shared/paging.hpp>

#include <userver/components/loggable_component_base.hpp>
#include <userver/utest/using_namespace_userver.hpp>
#include <userver/storages/postgres/cluster.hpp>

namespace svetit::project::table {

class CcModeType final {
public:
	explicit CcModeType(storages::postgres::ClusterPtr pg);
	model::CcModeType Select(int id);
	int Insert(
		int ccTypeId,
		const std::string& key,
		const std::string& name,
		bool isDeleted);
	void Update(const model::CcModeType& ccModeType);
	void Delete(int id);
	PagingResult<model::CcModeType> GetList(int start, int limit);
private:
	storages::postgres::ClusterPtr _pg;
};

} // namespace svetit::project::table
