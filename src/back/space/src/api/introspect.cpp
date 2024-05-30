#include "introspect.hpp"
#include "../service/service.hpp"
#include "../model/model.hpp"
#include <shared/headers.hpp>
#include <shared/errors.hpp>

#include <regex>
#include <boost/crc.hpp>

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

		//

		if (!req.HasHeader("X-Original-URI"))
			throw errors::BadRequest400("Header X-Original-URI missing");

		const std::string header = req.GetHeader("X-Original-URI");

		//

		std::regex rgx("^/([^/]+)/");
		std::smatch match;

		if (!std::regex_search(header.begin(), header.end(), match, rgx))
			throw errors::BadRequest400("Space name missing");

		const std::string spaceName = match[1];

		//

		boost::crc_32_type result;
    	result.process_bytes(spaceName.data(), spaceName.length());
    	const uint32_t spaceNameCRC32 = result.checksum();

		//
		const std::string cookieName = "space_" + std::to_string(spaceNameCRC32);

		if (req.HasCookie(cookieName)) {
			const std::string token = req.GetCookie(cookieName);
			LOG_WARNING() << "Cookie exists, token: " << token;

			try {
				SpaceTokenPayload data = _s.Tokens().Verify(token);
				LOG_WARNING() << "Token verify OK";
				req.GetHttpResponse().SetHeader(headers::kSpaceId, data._id);
				req.GetHttpResponse().SetHeader(headers::kSpaceRole, data._role);
				return "Ok";
			} catch(const std::exception& e) {
				// проверить конкретный тип исключения.
				LOG_WARNING() << "Fail to verify token: " << e.what();
			}
		}

		LOG_WARNING() << "Creating new token";
		// todo - need to get real data from DB and header X-User

		std::string userId = "11111111-1111-1111-1111-111111111111";
		std::string id = "11111111-1111-1111-1111-111111111111";
		std::string role = "admin";

		std::string token = _s.Tokens().Create(spaceName, id, role, userId);
		LOG_WARNING() << "Token created: " << token;
		server::http::Cookie cookie{cookieName, token};
		cookie.SetPath("/");
		cookie.SetSecure();
		cookie.SetHttpOnly();
		cookie.SetSameSite("Lax");

		auto& resp = req.GetHttpResponse();
		resp.SetCookie(cookie);

		req.GetHttpResponse().SetHeader(headers::kSpaceId, id);
		req.GetHttpResponse().SetHeader(headers::kSpaceRole, role);

		return "Ok";
	}
	catch(const std::exception& e) {
		LOG_WARNING() << "Fail to verify: " << e.what();
		req.SetResponseStatus(server::http::HttpStatus::kUnauthorized);
		return "Invalid space authorization token";
	}

}

} // namespace svetit::space::handlers
