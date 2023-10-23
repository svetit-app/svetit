#pragma once

#include "../model/model.hpp"

#include <string>
#include <string_view>
#include <vector>

#include <userver/components/loggable_component_base.hpp>
#include <userver/yaml_config/schema.hpp>
#include <userver/utest/using_namespace_userver.hpp>
#include <userver/http/url.hpp>

namespace svetit::space {

class Service final : public components::LoggableComponentBase {
public:
	static constexpr std::string_view kName = "main-service";
	static yaml_config::Schema GetStaticConfigSchema();

	explicit Service(
		const components::ComponentConfig& conf,
		const components::ComponentContext& ctx);

	std::vector<svetit::space::Space> GetList();

private:
	std::string _someUrl;
	std::vector<svetit::space::Space> _spaces;
};

} // namespace svetit::space
