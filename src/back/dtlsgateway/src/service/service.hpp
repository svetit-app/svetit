#pragma once

#include <svetit/paging.hpp>

#include <userver/components/loggable_component_base.hpp>
#include <userver/yaml_config/schema.hpp>
#include <userver/utest/using_namespace_userver.hpp>
#include <userver/http/url.hpp>
#include <userver/utils/boost_uuid4.hpp>

namespace svetit::dtlsgateway {

class RepositoryComponent;

class Service final : public components::LoggableComponentBase {
public:
	static constexpr std::string_view kName = "main-service";
	static yaml_config::Schema GetStaticConfigSchema();

	explicit Service(
		const components::ComponentConfig& conf,
		const components::ComponentContext& ctx);

	RepositoryComponent& Repo();

	const std::string& GetJSONSchemasPath();

private:
	RepositoryComponent& _repo;
	int _itemsLimitForList;
	std::string _jsonSchemasPath;
};

} // namespace svetit::dtlsgateway
