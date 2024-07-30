#pragma once

#include <userver/components/loggable_component_base.hpp>
#include <userver/yaml_config/schema.hpp>
#include <userver/utest/using_namespace_userver.hpp>

#include <shared/type_utils.hpp>
#include <shared/db/db_base.hpp>

namespace svetit::node {

class Repository final : public components::LoggableComponentBase {
public:
	static constexpr std::string_view kName = "repository";

	static yaml_config::Schema GetStaticConfigSchema();

	explicit Repository(
		const components::ComponentConfig& conf,
		const components::ComponentContext& ctx);

private:
	std::shared_ptr<db::Base> _db;
};

} // namespace svetit::node
