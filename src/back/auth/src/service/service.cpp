#include "service.hpp"
#include "tokenizer.hpp"
#include "oidc.hpp"
#include "../repo/repository.hpp"
#include "session.hpp"

#include <chrono>
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
	, _session{_rep.Session(), _tokenizer.Session()}
{
	auto issuer = _oidc.GetPrivateIssuer();
	auto jwks = _oidc.GetJWKS();
	_tokenizer.OIDC().SetJWKS(issuer, jwks);

	if (_webErrorPath.empty())
		_webErrorPath = _webLogoutPath;
}

service::Session& Service::Session() {
	return _session;
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

LoginCompletePayload Service::GetLoginCompleteUrl(
	const std::string& url,
	const std::string& state,
	const std::string& code,
	const std::string& userAgent,
	const std::string& redirectPath)
{
	// Обмениваем AUTH_CODE на токены OIDC
	const auto tokens = getTokens(state, code);

	// Получаем информацию о пользователе из токена
	const auto data = _tokenizer.OIDC().Parse(tokens._accessToken);

	// Получаем expiration time из refresh токена OIDC
	const std::chrono::system_clock::time_point exp = _tokenizer.OIDC().ParseExp(tokens._refreshToken);

	// Создаём и сохраняем сессию
	const auto session = _session.Create(tokens, data, userAgent, exp);

	// Генерируем ссылку для перенаправления пользователя
	http::Args args;
	if (!redirectPath.empty())
		args.emplace("redirectPath", redirectPath);

	return {
		._url = http::MakeUrl(url + _webLoginPath, args),
		._token = session._token
	};
}

std::string Service::GetLogoutUrl(
	const std::string& sessionId,
	const std::string& callbackUrl)
{
	// Получаем сессию из базы
	auto session = _session.Table().GetById(sessionId);

	// Обновляем OIDC токены если требуется
	if (_tokenizer.IsExpired(session._accessToken))
	{
		// TODO: dest lock, maybe?
		updateTokens(session);
	}

	_session.Table().MarkInactive(session);
	return _oidc.GetLogoutUrl(session._idToken, callbackUrl);
}

std::string Service::GetLogoutCompleteUrl(const std::string& url) const
{
	return url + _webLogoutPath;
}

OIDCTokens Service::TokenRefresh(const std::string& refreshToken)
{
	auto tokens = _oidc.Refresh(refreshToken);
	return tokens;
}

model::UserInfo Service::GetUserInfo(const std::string& sessionId)
{
	// Получаем сессию из базы
	auto session = _session.Table().GetById(sessionId);

	// Обновляем OIDC токены если требуется
	if (_tokenizer.IsExpired(session._accessToken))
	{
		// TODO: dest lock!
		updateTokens(session);
		_session.Table().UpdateTokens(session);
	}

	// Запрашиваем у OIDC инфу о пользователе
	return _oidc.GetUserInfo(session._accessToken);
}

OIDCTokens Service::getTokens(
	const std::string& state,
	const std::string& code)
{
	if (state.empty() || code.empty())
		throw std::runtime_error("state and code param can't be empty");

	auto redirectUrl = _rep.State().Take(state);

	auto tokens = _oidc.Exchange(code, redirectUrl);
	_tokenizer.OIDC().Verify(tokens._accessToken);
	return tokens;
}

void Service::updateTokens(model::Session& session) const
{
	auto tokens = _oidc.Refresh(session._refreshToken);
	session._accessToken = tokens._accessToken;
	session._refreshToken = tokens._refreshToken;
	session._idToken = tokens._idToken;
}

} // namespace svetit::auth
