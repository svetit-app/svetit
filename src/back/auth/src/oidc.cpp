#include "oidc.hpp"

#include <fmt/format.h>

#include <set>
#include <userver/yaml_config/merge_schemas.hpp>
#include <userver/clients/http/component.hpp>
#include "userver/components/component_config.hpp"
#include "userver/components/component_context.hpp"
#include <userver/formats/json/value.hpp>
#include <userver/http/url.hpp>
#include <userver/http/common_headers.hpp>
#include <userver/http/content_type.hpp>

namespace svetit::auth {

/*static*/ yaml_config::Schema OIDConnect::GetStaticConfigSchema()
{
	return yaml_config::MergeSchemas<components::LoggableComponentBase>(R"(
type: object
description: OpenID Connect component
additionalProperties: false
properties:
  client-id:
    type: string
    description: name of OIDC client
  client-secret:
    type: string
    description: secret key of OIDC client
  provider-url:
    type: string
    description: url of OIDC
)");
}

OIDConnect::OIDConnect(
		const components::ComponentConfig& conf,
		const components::ComponentContext& ctx)
	: components::LoggableComponentBase{conf, ctx}
	, _clientId{conf["client-id"].As<std::string>()}
	, _clientSecret{conf["client-secret"].As<std::string>()}
	, _http{ctx.FindComponent<components::HttpClient>().GetHttpClient()}
{
	auto providerUrl = conf["provider-url"].As<std::string>();
	auto wellKnownUrl = providerUrl + "/.well-known/openid-configuration";
	auto res = _http.CreateRequest()
		.get(wellKnownUrl)
		.retry(2)
		.timeout(std::chrono::milliseconds{5000})
		.perform();
	res->raise_for_status();
	auto wellKnown = formats::json::FromString(res->body_view());

	_urls = ProviderUrls{
		wellKnown["issuer"].As<std::string>(),
		wellKnown["authorization_endpoint"].As<std::string>(),
		wellKnown["end_session_endpoint"].As<std::string>(),
		wellKnown["token_endpoint"].As<std::string>(),
		wellKnown["jwks_uri"].As<std::string>(),
		wellKnown["userinfo_endpoint"].As<std::string>()
	};

	auto algoList = wellKnown["id_token_signing_alg_values_supported"];
	std::set<std::string> supportedAlgo{
		"RS256",
		"RS384",
		"RS512",
		"ES256",
		"ES384",
		"ES512",
		"PS256",
		"PS384",
		"PS512"
	};

	std::vector<std::string> algos;
	for (auto&& it : algoList)
	{
		auto algo = it.As<std::string>();
		if (supportedAlgo.find(algo) != supportedAlgo.cend())
			algos.push_back(std::move(algo));
	}
}

std::string OIDConnect::GetPrivateIssuer() const
{
	return _urls._issuer;
}

std::string OIDConnect::GetJWKS() const
{
	auto res = _http.CreateRequest()
		.get(_urls._jwks)
		.retry(2)
		.timeout(std::chrono::milliseconds{5000})
		.perform();
	res->raise_for_status();
	return res->body();
}

std::string OIDConnect::GetLoginUrl(
	const std::string& state,
	const std::string& redirectUrl) const
{
	http::Args args;
	args.emplace("response_type", "code");
	args.emplace("scope", "openid");
	args.emplace("client_id", _clientId);
	args.emplace("state", state);
	args.emplace("redirect_uri", redirectUrl);
	return http::MakeUrl(_urls._auth, args);
}

std::string OIDConnect::GetLogoutUrl(
	const std::string& idToken,
	const std::string& redirectUrl) const
{
	http::Args args;
	args.emplace("client_id", _clientId);
	args.emplace("post_logout_redirect_uri", redirectUrl);
	args.emplace("id_token_hint", idToken);
	args.emplace("state", "logout_complete");
	return http::MakeUrl(_urls._logout, args);
}

std::string OIDConnect::Exchange(
	const std::string& code,
	const std::string& redirectUrl) const
{
	http::Args args;
	args.emplace("grant_type", "authorization_code");
	args.emplace("scope", "openid");
	args.emplace("client_id", _clientId);
	args.emplace("client_secret", _clientSecret);
	args.emplace("code", code);
	args.emplace("redirect_uri", redirectUrl);
	auto query = http::MakeQuery(args);

	auto res = _http.CreateRequest()
		.post(_urls._token, std::move(query))
		.headers({{http::headers::kContentType,
			"application/x-www-form-urlencoded"}})
		.timeout(std::chrono::seconds{5})
		.perform();
	res->raise_for_status();
	return res->body();
}

std::string OIDConnect::Refresh(const std::string& refreshToken) const
{
	http::Args args;
	args.emplace("grant_type", "refresh_token");
	args.emplace("scope", "openid");
	args.emplace("client_id", _clientId);
	args.emplace("client_secret", _clientSecret);
	args.emplace("refresh_token", refreshToken);
	auto query = http::MakeQuery(args);

	auto res = _http.CreateRequest()
		.post(_urls._token, std::move(query))
		.headers({{http::headers::kContentType,
			"application/x-www-form-urlencoded"}})
		.timeout(std::chrono::seconds{5})
		.perform();
	res->raise_for_status();
	return res->body();
}

} // namespace svetit::auth
