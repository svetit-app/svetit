#include "login_callback.hpp"
#include "helpers.hpp"
#include "../service/service.hpp"

#include <exception>

#include <userver/http/common_headers.hpp>

namespace svetit::auth::handlers {

LoginCallback::LoginCallback(
	const components::ComponentConfig& conf,
	const components::ComponentContext& ctx)
	: server::handlers::HttpHandlerBase{conf, ctx}
	, _s{ctx.FindComponent<Service>()}
{}

std::string LoginCallback::HandleRequestThrow(
	const server::http::HttpRequest& req,
	server::request::RequestContext&) const
{
	auto code = req.GetArg("code");
	auto state = req.GetArg("state");

	std::string url = getCallerUrl(req);
	try {
		// Обмениваем AUTH_CODE на токены OIDC
		const auto tokens = _s.GetTokens(state, code);

		// Получаем информацию о пользователе из токена
		const auto data = _s.GetOIDCTokenPayload(tokens);

		// Создаём и сохраняем сессию
		const std::string userAgent = req.GetHeader(http::headers::kUserAgent);
		const auto session = _s.Session().Create(tokens, data, userAgent);

		// Генерируем ссылку с данными сессии
		const auto redirectPath = req.GetArg("redirectPath");
		const auto args = getSessionArgs(data, session._token, redirectPath);
		url = _s.GetLoginCompleteUrl(url, args);
	}
	catch (const std::exception& e) {
		LOG_WARNING() << "GetTokens error:" << e.what();
		url = _s.GetErrorPageUrl(url);
	}

	auto& response = req.GetHttpResponse();
	response.SetStatus(server::http::HttpStatus::kFound);
	response.SetHeader(http::headers::kLocation, url);
	return {};
}

http::Args LoginCallback::getSessionArgs(
	const TokenPayload& data,
	const std::string& sessionToken,
	const std::string& redirectPath) const
{
	http::Args args;
	if (!redirectPath.empty())
		args.emplace("redirectPath", redirectPath);

	args.emplace("session", sessionToken);
	args.emplace("userId", data._userId);
	args.emplace("userLogin", data._userLogin);
	args.emplace("username", data._userName);
	return args;
}

} // namespace svetit::auth::handlers
