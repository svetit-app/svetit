#include "service.hpp"
#include "tokenizer.hpp"
#include "oidc.hpp"
#include "../repo/repository.hpp"
#include "session.hpp"
#include "../model/session.hpp"
#include "../model/oidctokens.hpp"
#include "../model/errors.hpp"
#include "../../../shared/errors.hpp"

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
  items-limit-for-list:
    type: integer
    description: How many items list may contain
)");
}

Service::Service(
		const components::ComponentConfig& conf,
		const components::ComponentContext& ctx)
	: components::LoggableComponentBase{conf, ctx}
	, _webErrorPath{conf["web-error-page-path"].As<std::string>({})}
	, _webLoginPath{conf["web-login-path"].As<std::string>()}
	, _webLogoutPath{conf["web-logout-path"].As<std::string>()}
	, _itemsLimitForList{conf["items-limit-for-list"].As<int>()}
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

std::string Service::GetErrorPageUrl(const std::string& url, bool forceLogout) const
{
	return http::MakeUrl(url + _webErrorPath, {
		{"isErr", "1"},
		{"forceLogout", forceLogout ? "1" : "0"},
	});
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
	const auto exp_token = _tokenizer.GetExpirationTime(tokens._refreshToken);
	const auto exp = std::chrono::duration_cast<std::chrono::seconds>(exp_token.time_since_epoch()).count();

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
	auto session = _session.Table().Get(sessionId);

	// Обновляем OIDC токены если требуется
	if (_tokenizer.IsExpired(session._accessToken))
	{
		// TODO: dest lock, maybe?
		updateTokens(session);
	}

	_session.Table().MarkInactive(session._id);
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
	auto session = _session.Table().Get(sessionId);

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

model::SessionRefresh Service::RefreshSession(
	const std::string& sessionId,
	const std::string& userAgent)
{
	// запрашиваем сессию, не важно, активна она или нет
	auto session = _session.Table().Get(sessionId, std::nullopt);

	// проверка на несовпадение userAgent
	differentDeviceSecurityCheck(userAgent, session._device);

	// проверка на неактивность сессии
	if (!session._active)
	{
		_session.Table().MarkInactive(session._userId);
		throw errors::SecurityRisk{"Same inactive session."};
	}

	// Обновляем OIDC токены
	updateTokens(session);

	// Обновляем токен Сессии
	try {
		auto token = updateSession(session, userAgent);
		return {token};
	} catch(errors::SecurityRisk& e) {
		_session.Table().MarkInactive(session._userId);
		throw;
	}

	return {};
}

std::string Service::updateSession(
	const model::Session& session,
	const std::string& userAgent)
{
	const OIDCTokens tokens = {
		._accessToken = session._accessToken,
		._refreshToken = session._refreshToken,
		._idToken = session._idToken
	};

	const auto data = _tokenizer.OIDC().Parse(tokens._accessToken);

	const auto exp_token = _tokenizer.GetExpirationTime(tokens._refreshToken);
	const auto exp = std::chrono::duration_cast<std::chrono::seconds>(exp_token.time_since_epoch()).count();

	const auto newSession = _session.Refresh(tokens, data, userAgent, exp, session._id);

	return newSession._token;
}

void Service::differentDeviceSecurityCheck(
	const std::string& currentUserAgent,
	const std::string& oldUserAgent)
{
	if (currentUserAgent != oldUserAgent)
	{
		const auto agent = currentUserAgent.substr(0, 150);
		throw errors::SecurityRisk{"Different user agent: '" + agent + '\''};
	}
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

model::UserInfo Service::GetUserInfoById(const std::string& id, const std::string& sessionId)
{
	// Получаем сессию из базы
	auto session = _session.Table().Get(sessionId);

	// Обновляем OIDC токены если требуется
	if (_tokenizer.IsExpired(session._accessToken))
	{
		// TODO: dest lock!
		updateTokens(session);
		_session.Table().UpdateTokens(session);
	}

	// Запрашиваем у OIDC инфу о пользователе
	return _oidc.GetUserInfoById(id, session._accessToken);
}

std::vector<model::UserInfo> Service::GetUserInfoList(const std::string& search, const std::string& sessionId, unsigned int start, unsigned int limit) {
	// Получаем сессию из базы
	auto session = _session.Table().Get(sessionId);
	// Обновляем OIDC токены если требуется
	if (_tokenizer.IsExpired(session._accessToken))
	{
		// TODO: dest lock!
		updateTokens(session);
		_session.Table().UpdateTokens(session);
	}

	// Запрашиваем у OIDC инфу о пользователе
	return _oidc.GetUserInfoList(search, session._accessToken, start, limit);
}

} // namespace svetit::auth
