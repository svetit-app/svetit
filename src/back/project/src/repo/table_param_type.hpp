#pragma once

#include "../model/param_type.hpp"
#include <shared/paging.hpp>

#include <userver/components/loggable_component_base.hpp>
#include <userver/utest/using_namespace_userver.hpp>
#include <userver/storages/postgres/cluster.hpp>

namespace svetit::project::table {

class ParamType final {
public:
	explicit ParamType(storages::postgres::ClusterPtr pg);
	model::ParamType Select(int id);
	int Insert(
		int parentId,
		const std::string& key,
		const std::string& name,
		const std::string& description,
		ParamValueType::Type valueType);
	void Update(const model::ParamType& paramType);
	void Delete(int id);
	PagingResult<model::ParamType> GetList(int start, int limit);
private:
	storages::postgres::ClusterPtr _pg;
};

} // namespace svetit::project::table
