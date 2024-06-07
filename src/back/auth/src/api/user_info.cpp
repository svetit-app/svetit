#include "user_info.hpp"
#include "../model/userinfo_serialize.hpp"
#include "../service/service.hpp"
#include <shared/headers.hpp>

#include <stdexcept>

#include <userver/http/common_headers.hpp>

namespace svetit::auth::handlers {

UserInfo::UserInfo(
	const components::ComponentConfig& conf,
	const components::ComponentContext& ctx)
	: server::handlers::HttpHandlerJsonBase{conf, ctx}
	, _s{ctx.FindComponent<Service>()}
{}

formats::json::Value UserInfo::HandleRequestJsonThrow(
	const server::http::HttpRequest& req,
	const formats::json::Value& body,
	server::request::RequestContext&) const
{
	formats::json::ValueBuilder res;

	const auto& sessionId = req.GetHeader(headers::kSessionId);
	if (sessionId.empty()) {
		res["err"] = "Empty sessionId header";
		req.SetResponseStatus(server::http::HttpStatus::kUnauthorized);
		return res.ExtractValue();
	}

	try {
		res = _s.GetUserInfo(sessionId);
	}
	catch(const std::exception& e) {
		LOG_WARNING() << '[' << sessionId << "] Fail to get user info: " << e.what();
		req.SetResponseStatus(server::http::HttpStatus::kUnauthorized);
		res["err"] = "Invalid session";
	}

	return res.ExtractValue();
}

} // namespace svetit::auth::handlers
