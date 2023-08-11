#include "token_refresh.hpp"
#include "../service.hpp"

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
	if (!body.HasMember("token"))
	{
		req.SetResponseStatus(server::http::HttpStatus::kBadRequest);
		return formats::json::FromString(
			R"({"error": "missing required field 'token'"})");
	}

	auto token = body["token"].As<std::string>();
	Tokens tokens = _s.TokenRefresh(token);

	formats::json::ValueBuilder result;
	result["access"] = tokens._accessToken;
	result["refresh"] = tokens._refreshToken;
	result["logout"] = tokens._logoutToken;
	return result.ExtractValue();
}

} // namespace svetit::auth::handlers
