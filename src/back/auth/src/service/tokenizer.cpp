#include "tokenizer.hpp"

#include "tokens_oidc.hpp"
#include "tokens_session.hpp"

#include <fmt/format.h>

#include <chrono>
#include <memory>
#include <stdexcept>
#include <userver/yaml_config/merge_schemas.hpp>
#include <userver/clients/http/component.hpp>
#include "jwt-cpp/traits/kazuho-picojson/traits.h"
#include "tokens_session.hpp"
#include "userver/components/component_config.hpp"
#include "userver/components/component_context.hpp"
#include <userver/formats/json/value.hpp>
#include <userver/http/url.hpp>

#include <jwt-cpp/traits/kazuho-picojson/defaults.h>

namespace svetit::auth {

/*static*/ yaml_config::Schema Tokenizer::GetStaticConfigSchema()
{
	return yaml_config::MergeSchemas<components::LoggableComponentBase>(R"(
type: object
description: Tokenizer component
additionalProperties: false
properties:
  internalTlsKeyPath:
    type: string
    description: TLS Private Key path
)");
}

Tokenizer::Tokenizer(
		const components::ComponentConfig& conf,
		const components::ComponentContext& ctx)
	: components::LoggableComponentBase{conf, ctx}
	, _OIDC{}
	, _session{conf["internalTlsKeyPath"].As<std::string>()}
{
}

tokens::OIDC& Tokenizer::OIDC() {
	return _OIDC;
}

tokens::Session& Tokenizer::Session() {
	return _session;
}

bool Tokenizer::IsExpired(const std::string& token) const
{
	auto now = std::chrono::system_clock::now() + std::chrono::seconds(5);
	return now < GetExpirationTime(token);
}

std::chrono::system_clock::time_point Tokenizer::GetExpirationTime(const std::string& token) const
{
	auto data = jwt::decode(token);
	return data.get_expires_at();
}

} // namespace svetit::auth
