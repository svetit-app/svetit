#pragma once

#include "model/model.hpp"

#include <string>
#include <string_view>

#include <userver/components/loggable_component_base.hpp>
#include <userver/yaml_config/schema.hpp>
#include <userver/utest/using_namespace_userver.hpp>
#include <userver/storages/postgres/cluster.hpp>

namespace svetit::auth {

class Repository final : public components::LoggableComponentBase {
public:
	static constexpr std::string_view kName = "repository";

	static yaml_config::Schema GetStaticConfigSchema();

	explicit Repository(
		const components::ComponentConfig& conf,
		const components::ComponentContext& ctx);

	void SaveState(
		const std::string& state,
		const std::string& redirectUrl);

	std::string TakeState(const std::string& state);

private:
	storages::postgres::ClusterPtr _pg;
};

} // namespace svetit::auth


