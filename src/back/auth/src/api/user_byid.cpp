#include "user_byid.hpp"
#include "../service/service.hpp"
#include "../../../shared/headers.hpp"
#include "../../../shared/errors.hpp"
#include "../../../shared/errors_catchit.hpp"
#include "../model/userinfo_serialize.hpp"

#include <userver/http/common_headers.hpp>

namespace svetit::auth::handlers {

UserById::UserById(
	const components::ComponentConfig& conf,
	const components::ComponentContext& ctx)
	: server::handlers::HttpHandlerJsonBase{conf, ctx}
	, _s{ctx.FindComponent<Service>()}
{}

formats::json::Value UserById::HandleRequestJsonThrow(
	const server::http::HttpRequest& req,
	const formats::json::Value& body,
	server::request::RequestContext&) const
{
	formats::json::ValueBuilder res;

	try {
		const auto& sessionId = req.GetHeader(headers::kSessionId);
		if (sessionId.empty()) {
			throw errors::Unauthorized401{};
		}

		if (req.HasPathArg("id")) {
			const auto id = req.GetPathArg("id");
			res = _s.GetUserInfoById(id, sessionId);
		} else {
			throw errors::BadRequest400("No id param");
		}

	} catch(...) {
		return errors::CatchIt(req);
	}

	return res.ExtractValue();
}

} // namespace svetit::auth::handlers