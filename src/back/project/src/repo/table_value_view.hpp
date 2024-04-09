#pragma once

#include "../model/value_view.hpp"
#include <shared/paging.hpp>

#include <userver/components/loggable_component_base.hpp>
#include <userver/utest/using_namespace_userver.hpp>
#include <userver/storages/postgres/cluster.hpp>

namespace svetit::project::table {

class ValueView final {
public:
	explicit ValueView(storages::postgres::ClusterPtr pg);
	model::ValueView Select(int id);
	int Insert(
		int diTypeId,
		const std::string& value,
		const std::string& view,
		bool isDeleted);
	void Update(const model::ValueView& valueView);
	void Delete(int id);
	PagingResult<model::ValueView> GetList(int start, int limit);
private:
	storages::postgres::ClusterPtr _pg;
};

} // namespace svetit::project::table