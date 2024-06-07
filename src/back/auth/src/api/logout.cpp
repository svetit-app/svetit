#include "logout.hpp"
#include "helpers.hpp"
#include "../service/service.hpp"
#include "../model/consts.hpp"
#include <shared/headers.hpp>

#include "userver/http/common_headers.hpp"

namespace svetit::auth::handlers {

Logout::Logout(
	const components::ComponentConfig& conf,
	const components::ComponentContext& ctx)
	: server::handlers::HttpHandlerBase{conf, ctx}
	, _s{ctx.FindComponent<Service>()}
	, _mapHttpMethodToSchema{LoadSchemas(kName, _s.GetJSONSchemasPath())}
{}

std::string Logout::HandleRequestThrow(
	const server::http::HttpRequest& req,
	server::request::RequestContext&) const
{
	const formats::json::Value body;
	const auto params = ValidateRequest(_mapHttpMethodToSchema, req, body);

	std::string token;
	if (params.HasMember(Consts::SessionCookieName))
		token = params[Consts::SessionCookieName].As<std::string>();

	if (token.empty()) {
		req.SetResponseStatus(server::http::HttpStatus::kUnauthorized);
		return "Empty session cookie token";
	}

	auto callbackUrl = getCallerUrl(req, params, /*addApiPrefix*/true);
	callbackUrl += "/auth/logout/callback";

	std::string url;
	try {
		const auto data = _s.Session().Token().Verify(token);
		url = _s.GetLogoutUrl(data._sessionId, callbackUrl);
	}
	catch (const std::exception& e) {
		LOG_WARNING() << "GetLogoutUrl:" << e.what();
		url = getCallerUrl(req, params);
		url = _s.GetErrorPageUrl(url, /*forceLogout=*/true);
	}

	auto& response = req.GetHttpResponse();
	response.SetStatus(server::http::HttpStatus::kFound);
	response.SetHeader(http::headers::kLocation, url);
	return {};
}

} // namespace svetit::auth::handlers
