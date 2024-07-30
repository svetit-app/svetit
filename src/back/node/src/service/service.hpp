#pragma once

#include "../repo/repository.hpp"

#include <userver/components/loggable_component_base.hpp>
#include <userver/yaml_config/schema.hpp>
#include <userver/utest/using_namespace_userver.hpp>

namespace svetit::node {

class Repository;

class Service final : public components::LoggableComponentBase {
public:
	static constexpr std::string_view kName = "main-service";
	static yaml_config::Schema GetStaticConfigSchema();

	explicit Service(
		const components::ComponentConfig& conf,
		const components::ComponentContext& ctx);

	const std::string& GetJSONSchemasPath();

	Repository& Repo();

private:
	Repository& _repo;
	int _itemsLimitForList;
	std::string _jsonSchemasPath;
};

} // namespace svetit::project
