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
	const auto redirectPath = req.GetArg("redirectPath");
	const auto state = req.GetArg("state");
	const auto code = req.GetArg("code");

	const std::string userAgent = req.GetHeader(http::headers::kUserAgent);

	std::string url = getCallerUrl(req);
	try {
		auto data = _s.GetLoginCompleteUrl(url, state, code, userAgent, redirectPath);
		url = std::move(data._url);

		server::http::Cookie cookie{"session", data._token};
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
