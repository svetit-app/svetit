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
	model::DiType Get(const boost::uuids::uuid& spaceId, int64_t id);
	int64_t Create(const model::DiType& item);
	void Update(const model::DiType& item);
	void Delete(const boost::uuids::uuid& spaceId, int64_t id);
	PagingResult<model::DiType> GetList(const boost::uuids::uuid& spaceId, const boost::uuids::uuid& projectId, int start, int limit);
private:
	storages::postgres::ClusterPtr _pg;
};

} // namespace svetit::project::table
