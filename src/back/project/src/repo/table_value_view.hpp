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
	model::ValueView Get(int64_t id, const boost::uuids::uuid& spaceId);
	int64_t Create(const model::ValueView& valueView);
	void Update(const model::ValueView& valueView);
	void Delete(int64_t id, const boost::uuids::uuid& spaceId);
	PagingResult<model::ValueView> GetList(const boost::uuids::uuid& spaceId, int start, int limit);
private:
	storages::postgres::ClusterPtr _pg;
};

} // namespace svetit::project::table
