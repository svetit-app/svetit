#include "login_callback.hpp"
#include "helpers.hpp"
#include "../service/service.hpp"
#include "../model/consts.hpp"

#include <exception>

#include <userver/http/common_headers.hpp>

namespace svetit::auth::handlers {

LoginCallback::LoginCallback(
	const components::ComponentConfig& conf,
	const components::ComponentContext& ctx)
	: server::handlers::HttpHandlerBase{conf, ctx}
	, _s{ctx.FindComponent<Service>()}
	, _mapHttpMethodToSchema{LoadSchemas(kName, _s.GetJSONSchemasPath())}
{}

std::string LoginCallback::HandleRequestThrow(
	const server::http::HttpRequest& req,
	server::request::RequestContext&) const
{
	const auto params = ValidateRequest(_mapHttpMethodToSchema, req);

	// может быть, вот тут нужно проверять через HasMember? и если false, то инициализировать пустой строкой?
	const auto redirectPath = params["redirectPath"].As<std::string>();
	const auto state = params["state"].As<std::string>();
	const auto code = params["code"].As<std::string>();
	const std::string userAgent = params[http::headers::kUserAgent].As<std::string>();

	std::string url = getCallerUrl(req, params);
	try {
		auto data = _s.GetLoginCompleteUrl(url, state, code, userAgent, redirectPath);
		url = std::move(data._url);

		server::http::Cookie cookie{Consts::SessionCookieName, data._token};
		cookie.SetPath("/");
		cookie.SetSecure();
		cookie.SetHttpOnly();
		cookie.SetSameSite("Lax");

		auto& resp = req.GetHttpResponse();
		resp.SetCookie(cookie);
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

} // namespace svetit::auth::handlers
