#pragma once

#include "../model/cc_type_param.hpp"
#include <shared/paging.hpp>

#include <userver/components/loggable_component_base.hpp>
#include <userver/utest/using_namespace_userver.hpp>
#include <userver/storages/postgres/cluster.hpp>

namespace svetit::project::table {

class CcTypeParam final {
public:
	explicit CcTypeParam(storages::postgres::ClusterPtr pg);
	model::CcTypeParam Select(int ccTypeId, int paramId);
	void Insert(
		int ccTypeId,
		int paramId,
		bool isDeleted);
	void Update(const model::CcTypeParam& ccTypeParam);
	void Delete(int ccTypeId, int paramId);
	PagingResult<model::CcTypeParam> GetList(int start, int limit);
private:
	storages::postgres::ClusterPtr _pg;
};

} // namespace svetit::project::table
