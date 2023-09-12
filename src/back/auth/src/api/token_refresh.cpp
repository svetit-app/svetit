#include "token_refresh.hpp"
#include "../service/service.hpp"
#include "../../../shared/headers.hpp"
#include "../model/session_refresh_serialize.hpp"
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
	formats::json::ValueBuilder res;

	const auto& sessionId = req.GetHeader(headers::kSessionId);
	if (sessionId.empty()) {
		res["err"] = "Empty sessionId header";
		return res.ExtractValue();
	}

	const std::string userAgent = req.GetHeader(http::headers::kUserAgent);

	try {
		res = _s.RefreshSession(sessionId, userAgent);
	}
	catch(const std::exception& e) {
		LOG_WARNING() << '[' << sessionId << "] Fail to refresh session token: " << e.what();
		req.SetResponseStatus(server::http::HttpStatus::kUnauthorized);
		res["err"] = "Invalid session";
	}

	return res.ExtractValue();
}

} // namespace svetit::auth::handlers