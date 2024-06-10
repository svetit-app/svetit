#include "logout_callback.hpp"
#include "helpers.hpp"
#include "../service/service.hpp"

#include "userver/http/common_headers.hpp"

namespace svetit::auth::handlers {

LogoutCallback::LogoutCallback(
	const components::ComponentConfig& conf,
	const components::ComponentContext& ctx)
	: server::handlers::HttpHandlerBase{conf, ctx}
	, _s{ctx.FindComponent<Service>()}
	, _mapHttpMethodToSchema{LoadSchemas(kName, _s.GetJSONSchemasPath())}
{}

std::string LogoutCallback::HandleRequestThrow(
	const server::http::HttpRequest& req,
	server::request::RequestContext&) const
{
	const auto params = ValidateRequest(_mapHttpMethodToSchema, req);
	
	auto url = getCallerUrl(req, params);
	try {
		url = _s.GetLogoutCompleteUrl(url);
	}
	catch (const std::exception& e) {
		LOG_WARNING() << "GetLogoutUrl:" << e.what();
		url = _s.GetErrorPageUrl(url, /*forceLogout=*/true);
	}

	auto& response = req.GetHttpResponse();
	response.SetStatus(server::http::HttpStatus::kFound);
	response.SetHeader(http::headers::kLocation, url);
	return {};
}

} // namespace svetit::auth::handlers
