#pragma once

#include "../model/section_param.hpp"
#include <shared/paging.hpp>

#include <userver/components/loggable_component_base.hpp>
#include <userver/utest/using_namespace_userver.hpp>
#include <userver/storages/postgres/cluster.hpp>

namespace svetit::project::table {

class SectionParam final {
public:
	explicit SectionParam(storages::postgres::ClusterPtr pg);
	model::SectionParam Get(int64_t sectionId, int64_t paramId);
	void Insert(int64_t sectionId, int64_t paramId);
	void Delete(int64_t sectionId, int64_t paramId);
	PagingResult<model::SectionParam> GetList(int start, int limit);
private:
	storages::postgres::ClusterPtr _pg;
};

} // namespace svetit::project::table
