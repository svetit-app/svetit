#include "user_info.hpp"
#include "../model/userinfo_serialize.hpp"
#include "../service/service.hpp"
#include <svetit/headers.hpp>

#include <stdexcept>

#include <userver/http/common_headers.hpp>

namespace svetit::auth::handlers {

UserInfo::UserInfo(
	const components::ComponentConfig& conf,
	const components::ComponentContext& ctx)
	: server::handlers::HttpHandlerJsonBase{conf, ctx}
	, _s{ctx.FindComponent<Service>()}
	, _mapHttpMethodToSchema{LoadSchemas(kName, _s.GetJSONSchemasPath())}
{}

formats::json::Value UserInfo::HandleRequestJsonThrow(
	const server::http::HttpRequest& req,
	const formats::json::Value& body,
	server::request::RequestContext&) const
{
	formats::json::ValueBuilder res;

	const auto params = ValidateRequest(_mapHttpMethodToSchema, req, body);
	const auto sessionId = params[headers::kSessionId].As<std::string>();

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
