#include "introspect.hpp"
#include "../service/service.hpp"
#include <shared/headers.hpp>
#include <shared/errors.hpp>

#include <regex>

#include <userver/http/common_headers.hpp>

namespace svetit::space::handlers {

Introspect::Introspect(
	const components::ComponentConfig& conf,
	const components::ComponentContext& ctx)
	: server::handlers::HttpHandlerBase{conf, ctx}
	, _s{ctx.FindComponent<Service>()}
{}

std::string Introspect::HandleRequestThrow(
	const server::http::HttpRequest& req,
	server::request::RequestContext&) const
{
	try {
		if (!req.HasHeader("X-Original-URI"))
			throw errors::BadRequest400("Header X-Original-URI missing");

		const std::string header = req.GetHeader("X-Original-URI");

		std::regex rgx("^/([^/]+)/");
		std::smatch match;

		if (!std::regex_search(header.begin(), header.end(), match, rgx))
			throw errors::BadRequest400("Space name missing");

		const std::string spaceName = match[1];

		//const auto data = _s.Tokens().Verify(token);

		req.GetHttpResponse().SetHeader(headers::kSpaceId, "11111111-1111-1111-1111-111111111111");
		req.GetHttpResponse().SetHeader(headers::kSpaceRole, "admin");
	}
	catch(const std::exception& e) {
		LOG_WARNING() << "Fail to verify: " << e.what();
		req.SetResponseStatus(server::http::HttpStatus::kUnauthorized);
		return "Invalid space authorization token";
	}
	return "Ok";
}

} // namespace svetit::space::handlers
