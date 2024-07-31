#pragma once

#include "../repo/repository.hpp"
#include "../model/node.hpp"

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

	model::Node Get(const boost::uuids::uuid& id, const std::string& userId);
	void Delete(const boost::uuids::uuid& id, const std::string& userId);
	void Create(const model::Node& item, const std::string& userId);
	void Update(const model::Node& item, const std::string& userId);

private:
	Repository& _repo;
	int _itemsLimitForList;
	std::string _jsonSchemasPath;
};

} // namespace svetit::project
