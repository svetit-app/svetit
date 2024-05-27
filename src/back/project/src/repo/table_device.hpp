#pragma once

#include "../model/device.hpp"
#include <shared/paging.hpp>

#include <userver/components/loggable_component_base.hpp>
#include <userver/utest/using_namespace_userver.hpp>
#include <userver/storages/postgres/cluster.hpp>

namespace svetit::project::table {

class Device final {
public:
	explicit Device(storages::postgres::ClusterPtr pg);
	model::Device Get(const boost::uuids::uuid& spaceId, int64_t id);
	int64_t Create(const model::Device& item);
	void Update(const model::Device& item);
	void Delete(const boost::uuids::uuid& spaceId, int64_t id);
	PagingResult<model::Device> GetList(const boost::uuids::uuid& spaceId, const boost::uuids::uuid& projectId, int start, int limit);
private:
	storages::postgres::ClusterPtr _pg;
};

} // namespace svetit::project::table
