#include "tokenizer.hpp"

#include "tokens_oidc.hpp"
#include "tokens_internal.hpp"

#include <fmt/format.h>

#include <memory>
#include <stdexcept>
#include <userver/yaml_config/merge_schemas.hpp>
#include <userver/clients/http/component.hpp>
#include "jwt-cpp/traits/kazuho-picojson/traits.h"
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
  some-var:
    type: string
    description: some desc
)");
}

Tokenizer::Tokenizer(
		const components::ComponentConfig& conf,
		const components::ComponentContext& ctx)
	: components::LoggableComponentBase{conf, ctx}
	, _OIDC{svetit::auth::tokens::OIDC()}
	, _internalTokens{svetit::auth::tokens::InternalTokens()}
{
}

svetit::auth::tokens::OIDC& Tokenizer::OIDC(){
	return _OIDC;
}

svetit::auth::tokens::InternalTokens& Tokenizer::InternalTokens(){
	return _internalTokens;
}

} // namespace svetit::auth
