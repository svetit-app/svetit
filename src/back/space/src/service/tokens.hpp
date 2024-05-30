#pragma once

#include <memory>
#include <string>
#include <string_view>

#include "../model/model.hpp"

#include <userver/components/loggable_component_base.hpp>
#include <userver/yaml_config/schema.hpp>
#include <userver/clients/http/client.hpp>
#include <userver/utest/using_namespace_userver.hpp>

namespace svetit::space::tokens {

struct jwt_session_impl;

class Tokens final: public components::LoggableComponentBase {
	static constexpr std::string_view _issuer = "svetit";
public:
	static constexpr std::string_view kName = "tokens";

	static yaml_config::Schema GetStaticConfigSchema();

	explicit Tokens(
		const components::ComponentConfig& conf,
		const components::ComponentContext& ctx);

	std::string Create(
		const std::string& key,
		const std::string& id,
		const std::string& role,
		const std::string& userId,
		int sec);
	SpaceTokenPayload Verify(const std::string& token);

private:
	std::string readKey(const std::string& path) const;

	std::shared_ptr<jwt_session_impl> _jwt;
	std::string _privateKeyPath;
};

} // namespace svetit::auth::tokens
