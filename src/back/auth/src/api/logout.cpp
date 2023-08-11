#include "logout.hpp"
#include "helpers.hpp"
#include "../service.hpp"

#include "userver/http/common_headers.hpp"

namespace svetit::auth::handlers {

Logout::Logout(
	const components::ComponentConfig& conf,
	const components::ComponentContext& ctx)
	: server::handlers::HttpHandlerBase{conf, ctx}
	, _s{ctx.FindComponent<Service>()}
{}

std::string Logout::HandleRequestThrow(
	const server::http::HttpRequest& req,
	server::request::RequestContext&) const
{
	auto token = req.GetArg("token");

	auto callbackUrl = getCallerUrl(req, /*addApiPrefix*/true);
	callbackUrl += "/auth/logout/callback";

	std::string url;
	try {
		url = _s.GetLogoutUrl(token, callbackUrl);
	}
	catch (const std::exception& e) {
		LOG_WARNING() << "GetLogoutUrl:" << e.what();
		url = getCallerUrl(req);
		url = _s.GetErrorPageUrl(url);
	}

	auto& response = req.GetHttpResponse();
	response.SetStatus(server::http::HttpStatus::kFound);
	response.SetHeader(http::headers::kLocation, url);
	return {};
}

} // namespace svetit::auth::handlers
