#include "token_refresh.hpp"
#include "../model/session_refresh_serialize.hpp"
#include "../service/service.hpp"
#include <svetit/headers.hpp>

#include <userver/http/common_headers.hpp>

namespace svetit::auth::handlers {

TokenRefresh::TokenRefresh(
	const components::ComponentConfig& conf,
	const components::ComponentContext& ctx)
	: server::handlers::HttpHandlerJsonBase{conf, ctx}
	, _s{ctx.FindComponent<Service>()}
	, _mapHttpMethodToSchema{LoadSchemas(kName, _s.GetJSONSchemasPath())}
{}

formats::json::Value TokenRefresh::HandleRequestJsonThrow(
	const server::http::HttpRequest& req,
	const formats::json::Value& body,
	server::request::RequestContext&) const
{
	formats::json::ValueBuilder res;

	const auto params = ValidateRequest(_mapHttpMethodToSchema, req, body);
	const auto sessionId = params[headers::kSessionId].As<std::string>();

	const std::string userAgent = params[http::headers::kUserAgent].As<std::string>();

	try {
		res = _s.RefreshSession(sessionId, userAgent);
		req.SetResponseStatus(server::http::HttpStatus::kNoContent);
	}
	catch(const std::exception& e) {
		LOG_WARNING() << '[' << sessionId << "] Fail to refresh session token: " << e.what();
		req.SetResponseStatus(server::http::HttpStatus::kUnauthorized);
		res["err"] = "Invalid session";
	}

	return res.ExtractValue();
}

} // namespace svetit::auth::handlers
