#include "token_refresh.hpp"
#include "../service/service.hpp"
#include "../../../shared/headers.hpp"
#include "../model/session_serialize.hpp"
#include "userver/http/common_headers.hpp"

namespace svetit::auth::handlers {

TokenRefresh::TokenRefresh(
	const components::ComponentConfig& conf,
	const components::ComponentContext& ctx)
	: server::handlers::HttpHandlerJsonBase{conf, ctx}
	, _s{ctx.FindComponent<Service>()}
{}

formats::json::Value TokenRefresh::HandleRequestJsonThrow(
	const server::http::HttpRequest& req,
	const formats::json::Value& body,
	server::request::RequestContext&) const
{
	/*if (!body.HasMember("token"))
	{
		req.SetResponseStatus(server::http::HttpStatus::kBadRequest);
		return formats::json::FromString(
			R"({"error": "missing required field 'token'"})");
	}*/

	formats::json::ValueBuilder res;

	const auto& sessionId = req.GetHeader(headers::kSessionId);
	if (sessionId.empty()) {
		res["err"] = "Empty sessionId header";
		return res.ExtractValue();
	}

	try {
		res = _s.RefreshSession(sessionId);
	}
	catch(const std::exception& e) {
		LOG_WARNING() << "Fail to refresh session token: " << e.what();
		req.SetResponseStatus(server::http::HttpStatus::kUnauthorized);
		res["err"] = "Invalid session";
	}

	return res.ExtractValue();
}

} // namespace svetit::auth::handlers
