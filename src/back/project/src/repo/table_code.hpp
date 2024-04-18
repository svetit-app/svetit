#pragma once

#include "../model/code.hpp"
#include <shared/paging.hpp>

#include <userver/components/loggable_component_base.hpp>
#include <userver/utest/using_namespace_userver.hpp>
#include <userver/storages/postgres/cluster.hpp>

namespace svetit::project::table {

class Code final {
public:
	explicit Code(storages::postgres::ClusterPtr pg);
	model::Code Select(int id);
	void Insert(
		const boost::uuids::uuid& projectId,
		const boost::uuids::uuid& repositoryId,
		const std::string& commitHash);
	void Update(const model::Code& code);
	void Delete(int id);
	PagingResult<model::Code> GetList(int start, int limit);
private:
	storages::postgres::ClusterPtr _pg;
};

} // namespace svetit::project::table
