#include "service.hpp"
#include "tokenizer.hpp"
#include "oidc.hpp"
#include "../repo/repository.hpp"
#include "session.hpp"

#include <limits>
#include <stdexcept>
#include <userver/yaml_config/merge_schemas.hpp>
#include <userver/clients/http/component.hpp>
#include "userver/components/component_config.hpp"
#include "userver/components/component_context.hpp"
#include "userver/formats/json/serialize.hpp"
#include "userver/http/url.hpp"
#include <userver/crypto/base64.hpp>
#include <userver/utils/rand.hpp>

namespace svetit::auth {

/*static*/ yaml_config::Schema Service::GetStaticConfigSchema()
{
	return yaml_config::MergeSchemas<components::LoggableComponentBase>(R"(
type: object
description: Main service component
additionalProperties: false
properties:
  web-error-page-path:
    type: string
    description: Path to web auth error page. If empty, using web-logout-path
    defaultDescription: ''
  web-login-path:
    type: string
    description: Path for web client login finalize
  web-logout-path:
    type: string
    description: Path for web client logout finalize
)");
}

Service::Service(
		const components::ComponentConfig& conf,
		const components::ComponentContext& ctx)
	: components::LoggableComponentBase{conf, ctx}
	, _webErrorPath{conf["web-error-page-path"].As<std::string>({})}
	, _webLoginPath{conf["web-login-path"].As<std::string>()}
	, _webLogoutPath{conf["web-logout-path"].As<std::string>()}
	, _tokenizer{ctx.FindComponent<Tokenizer>()}
	, _oidc{ctx.FindComponent<OIDConnect>()}
	, _rep{ctx.FindComponent<Repository>()}
	, _session{_rep,_tokenizer}
{
	auto issuer = _oidc.GetPrivateIssuer();
	auto jwks = _oidc.GetJWKS();
	_tokenizer.OIDC().SetJWKS(issuer, jwks);

	if (_webErrorPath.empty())
		_webErrorPath = _webLogoutPath;
}

std::string generateRandomHash(std::size_t len = 64) {
	std::string data(len, '0');
	for (auto& c : data)
		c = utils::RandRange(
			std::numeric_limits<std::string::value_type>::min(),
			std::numeric_limits<std::string::value_type>::max());
	return crypto::base64::Base64UrlEncode(data);
}

std::string Service::GetErrorPageUrl(const std::string& url) const
{
	return http::MakeUrl(url + _webErrorPath, {{"isErr", "1"}});
}

std::string Service::GetLoginUrl(const std::string& callbackUrl) const
{
	auto state = generateRandomHash();
	_rep.State().Save(state, callbackUrl);
	return _oidc.GetLoginUrl(state, callbackUrl);
}

std::string Service::GetLoginCompleteUrl(
	const TokensAndUserData& data,
	const std::string& url,
	const std::string& redirectPath) const
{
	http::Args args;
	if (!redirectPath.empty())
		args.emplace("redirectPath", redirectPath);

	args.emplace("session", data._sessionToken);
	args.emplace("userid", data._userId);
	args.emplace("userlogin", data._userLogin);
	args.emplace("username", data._userName);

	return http::MakeUrl(url + _webLoginPath, args);
}

std::string Service::GetLogoutUrl(
	const std::string& idToken,
	const std::string& callbackUrl) const
{
	return _oidc.GetLogoutUrl(idToken, callbackUrl);
}

std::string Service::GetLogoutCompleteUrl(const std::string& url) const
{
	return url + _webLogoutPath;
}

auto Service::GetTokenPayload(const std::string& token) const
{
	const auto data = _tokenizer.OIDC().Parse(token);
	return data;
}

TokensAndUserData Service::GetTokens(
	const std::string& state,
	const std::string& code,
	const std::string& userAgent)
{
	if (state.empty() || code.empty())
		throw std::runtime_error("state and code param can't be empty");

	auto redirectUrl = _rep.State().Take(state);
	
	auto raw = _oidc.Exchange(code, redirectUrl);
	auto data = formats::json::FromString(raw);

	TokensAndUserData result = {
		._accessToken = data["access_token"].As<std::string>(),
		._refreshToken = data["refresh_token"].As<std::string>(),
		._logoutToken = data["id_token"].As<std::string>()
	};

	auto tokenPayload = Service::GetTokenPayload(result._accessToken);

	std::string userId = tokenPayload._userId;
	std::string device = userAgent; // todo: is user-agent enough for device detection?
	std::string sessionToken = _session.Save(userId, device, result._accessToken, result._refreshToken, result._logoutToken);

	result._sessionToken = sessionToken;
	result._userId = userId;
	result._userLogin = tokenPayload._userLogin;
	result._userName = tokenPayload._userName;

	return result;
}

Tokens Service::TokenRefresh(const std::string& refreshToken)
{
	auto raw = _oidc.Refresh(refreshToken);
	auto data = formats::json::FromString(raw);
	return Tokens{
		._accessToken = data["access_token"].As<std::string>(),
		._refreshToken = data["refresh_token"].As<std::string>(),
		._logoutToken = data["id_token"].As<std::string>()
	};
}

std::string Service::GetTokenUserId(const std::string& token) const
{
	const auto data = _tokenizer.OIDC().Parse(token);
	return data._userId;
}

} // namespace svetit::auth
