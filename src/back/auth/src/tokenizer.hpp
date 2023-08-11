#pragma once

#include <map>
#include <string>
#include <string_view>

#include <userver/components/loggable_component_base.hpp>
#include <userver/yaml_config/schema.hpp>
#include <userver/clients/http/client.hpp>
#include <userver/utest/using_namespace_userver.hpp>

#include "model/model.hpp"

namespace svetit::auth {

struct jwt_impl;

class Tokenizer final : public components::LoggableComponentBase {
public:
	static constexpr std::string_view kName = "tokenizer";

	static yaml_config::Schema GetStaticConfigSchema();

	explicit Tokenizer(
		const components::ComponentConfig& conf,
		const components::ComponentContext& ctx);

	void SetJWKS(
		const std::string& issuer,
		const std::string& raw);

	TokenPayload Parse(const std::string& raw);

private:
	std::shared_ptr<jwt_impl> _jwt;
};

} // namespace svetit::auth

