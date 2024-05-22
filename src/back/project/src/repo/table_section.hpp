#pragma once

#include "../model/section.hpp"
#include <shared/paging.hpp>

#include <userver/components/loggable_component_base.hpp>
#include <userver/utest/using_namespace_userver.hpp>
#include <userver/storages/postgres/cluster.hpp>

namespace svetit::project::table {

class Section final {
public:
	explicit Section(storages::postgres::ClusterPtr pg);
	model::Section Get(const boost::uuids::uuid& spaceId, int64_t id);
	int64_t Create(const model::Section& item);
	void Update(const model::Section& section);
	void Delete(const boost::uuids::uuid& spaceId, int64_t id);
	PagingResult<model::Section> GetList(const boost::uuids::uuid& spaceId, int start, int limit);
private:
	storages::postgres::ClusterPtr _pg;
};

} // namespace svetit::project::table
