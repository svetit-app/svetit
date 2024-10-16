#include "token_introspect.hpp"
#include "../service/service.hpp"
#include "../model/consts.hpp"
#include <svetit/headers.hpp>

#include <userver/http/common_headers.hpp>

namespace svetit::auth::handlers {

TokenIntrospect::TokenIntrospect(
	const components::ComponentConfig& conf,
	const components::ComponentContext& ctx)
	: server::handlers::HttpHandlerBase{conf, ctx}
	, _s{ctx.FindComponent<Service>()}
	, _mapHttpMethodToSchema{LoadSchemas(kName, _s.GetJSONSchemasPath())}
{}

std::string TokenIntrospect::HandleRequestThrow(
	const server::http::HttpRequest& req,
	server::request::RequestContext&) const
{
	const auto params = ValidateRequest(_mapHttpMethodToSchema, req);
	const auto token = params[Consts::SessionCookieName].As<std::string>("");
	const auto userAgent = params[http::headers::kUserAgent].As<std::string>();

	try {
		const auto data = _s.Session().Token().Verify(token);
		_s.IntrospectUserAgentCheck(data._sessionId, userAgent);

		req.GetHttpResponse().SetHeader(headers::kUserId, data._userId);
		req.GetHttpResponse().SetHeader(headers::kSessionId, data._sessionId);
	}
	catch(const std::exception& e) {
		LOG_WARNING() << "Fail to get user ID from token: " << e.what();
		req.SetResponseStatus(server::http::HttpStatus::kUnauthorized);
		return "Invalid authorization token";
	}
	return "Ok";
}

} // namespace svetit::auth::handlers
