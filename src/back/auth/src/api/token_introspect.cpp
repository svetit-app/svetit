#include "token_introspect.hpp"
#include "../service/service.hpp"
#include <shared/headers.hpp>

#include "userver/http/common_headers.hpp"

namespace svetit::auth::handlers {

TokenIntrospect::TokenIntrospect(
	const components::ComponentConfig& conf,
	const components::ComponentContext& ctx)
	: server::handlers::HttpHandlerBase{conf, ctx}
	, _s{ctx.FindComponent<Service>()}
{}

std::string TokenIntrospect::HandleRequestThrow(
	const server::http::HttpRequest& req,
	server::request::RequestContext&) const
{
	const auto& header = req.GetHeader(http::headers::kAuthorization);
	if (header.empty()) {
		req.SetResponseStatus(server::http::HttpStatus::kUnauthorized);
		return "Empty 'Authorization' header";
	}

	const auto pos = header.find(' ');
	if (pos == std::string::npos
		|| std::string_view{header.data(), pos} != "Bearer") {
		req.SetResponseStatus(server::http::HttpStatus::kUnauthorized);
		return "'Authorization' header should have 'Bearer some-token' format";
	}

	const std::string token{header.data() + pos + 1};
	try {
		const auto data = _s.Session().Token().Verify(token);

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
