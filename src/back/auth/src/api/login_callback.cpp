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
	auto redirectPath = req.GetArg("redirectPath");

	std::string url = getCallerUrl(req);
	try {
		auto tokens = _s.GetTokens(state, code);
		url = _s.GetLoginCompleteUrl(tokens, url, redirectPath);
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
