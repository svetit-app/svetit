#pragma once

#include "../model/project.hpp"
#include "../model/project_param.hpp"
#include "../model/section.hpp"
#include <shared/paging.hpp>

#include <userver/components/loggable_component_base.hpp>
#include <userver/yaml_config/schema.hpp>
#include <userver/utest/using_namespace_userver.hpp>
#include <userver/http/url.hpp>
#include <userver/utils/boost_uuid4.hpp>

namespace svetit::project {

class Repository;

class Service final : public components::LoggableComponentBase {
public:
	static constexpr std::string_view kName = "main-service";
	static yaml_config::Schema GetStaticConfigSchema();

	explicit Service(
		const components::ComponentConfig& conf,
		const components::ComponentContext& ctx);

	model::Project GetProjectById(const boost::uuids::uuid& id);
	model::Project GetProjectByKey(const std::string& key);
	void CreateProject(const model::Project& project);
	void UpdateProject(const model::Project& project);
	void DeleteProject(const boost::uuids::uuid& id);
	PagingResult<model::Project> GetProjectList(uint32_t start, uint32_t limit);

	model::ProjectParam GetProjectParam(const boost::uuids::uuid& projectId, int paramId);
	void CreateProjectParam(const model::ProjectParam& projectParam);
	void UpdateProjectParam(const model::ProjectParam& projectParam);
	void DeleteProjectParam(const boost::uuids::uuid& projectId, int paramId);
	PagingResult<model::ProjectParam> GetProjectParamList(uint32_t start, uint32_t limit, bool keepDeleted);

	model::Section GetSection(int id);
	void CreateSection(const model::Section& section);
	void UpdateSection(const model::Section& section);
	void DeleteSection(int id);
	PagingResult<model::Section> GetSectionList(uint32_t start, uint32_t limit, bool keepDeleted);

private:
	Repository& _repo;
	int _itemsLimitForList;
};

} // namespace svetit::project