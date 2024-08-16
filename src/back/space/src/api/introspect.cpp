#include "introspect.hpp"
#include "../service/service.hpp"
#include "../model/model.hpp"
#include "../model/consts.hpp"
#include <shared/headers.hpp>
#include <shared/errors.hpp>

#include <boost/uuid/uuid_io.hpp>

#include <userver/utils/boost_uuid4.hpp>
#include <userver/http/common_headers.hpp>

namespace svetit::space::handlers {

Introspect::Introspect(
	const components::ComponentConfig& conf,
	const components::ComponentContext& ctx)
	: server::handlers::HttpHandlerBase{conf, ctx}
	, _s{ctx.FindComponent<Service>()}
	, _mapHttpMethodToSchema{LoadSchemas(kName, _s.GetJSONSchemasPath())}
{}

std::string Introspect::HandleRequestThrow(
	const server::http::HttpRequest& req,
	server::request::RequestContext&) const
{
	try {
		const auto params = ValidateRequest(_mapHttpMethodToSchema, req);
		const auto header = params["X-Original-URI"].As<std::string>();
		const std::string spaceKey = _s.GetKeyFromHeader(header);
		const std::string cookieName = "space";

		if (params.HasMember(cookieName)) {
			const auto token = params[cookieName].As<std::string>();

			try {
				SpaceTokenPayload data = _s.Tokens().Verify(token);
				req.GetHttpResponse().SetHeader(headers::kSpaceId, data._id);
				const auto roleId = stoi(data._roleId);
				const auto isAdmin = (roleId == consts::kRoleAdmin) ? "1" : "0";
				req.GetHttpResponse().SetHeader(headers::kSpaceIsAdmin, isAdmin);
				req.SetResponseStatus(server::http::HttpStatus::kNoContent);
				return {};
			} catch(const std::exception& e) {
				LOG_DEBUG() << "Fail to verify token: " << e.what();
			}
		}

		auto userId = params[headers::kUserId].As<std::string>();
		const auto [space, roleId] = _s.GetSpaceAndRoleId(spaceKey, userId);
		const auto isAdmin = (roleId == consts::kRoleAdmin) ? "1" : "0";
		const std::string spaceIdStr = boost::uuids::to_string(space.id);
		const std::string token = _s.CreateToken(spaceIdStr, space.key, userId, std::to_string(roleId));

		const auto apiPrefix = params["X-ApiPrefix"].As<std::string>();
		const std::string path = apiPrefix + "/s/" + spaceKey + "/";

		server::http::Cookie cookie{cookieName, token};
		cookie.SetPath(path);
		cookie.SetSecure();
		cookie.SetHttpOnly();
		cookie.SetSameSite("Lax");

		auto& resp = req.GetHttpResponse();
		resp.SetCookie(cookie);
		req.GetHttpResponse().SetHeader(headers::kSpaceId, spaceIdStr);
		req.GetHttpResponse().SetHeader(headers::kSpaceIsAdmin, isAdmin);
		req.SetResponseStatus(server::http::HttpStatus::kNoContent);
	}
	catch(const std::exception& e) {
		LOG_WARNING() << "Fail to verify: " << e.what();
		req.SetResponseStatus(server::http::HttpStatus::kUnauthorized);
		return "Invalid space authorization token";
	}
	return {};
}

} // namespace svetit::space::handlers