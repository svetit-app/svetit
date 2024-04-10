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
	model::Section Select(int id);
	int Insert(
		const boost::uuids::uuid& projectId,
		const std::string& name,
		bool isDeleted);
	void Update(const model::Section& section);
	void Delete(int id);
	PagingResult<model::Section> GetList(int start, int limit);
	PagingResult<model::Section> GetListNoDeleted(int start, int limit);
private:
	storages::postgres::ClusterPtr _pg;
};

} // namespace svetit::project::table
