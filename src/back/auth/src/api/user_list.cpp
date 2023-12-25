#include "user_list.hpp"
#include "../model/userinfo_serialize.hpp"
#include "../service/service.hpp"
#include "../../../shared/headers.hpp"
#include "../../../shared/errors.hpp"
#include "../../../shared/errors_catchit.hpp"
#include "../../../shared/paging.hpp"

#include <stdexcept>

#include <userver/http/common_headers.hpp>

namespace svetit::auth::handlers {

UserList::UserList(
	const components::ComponentConfig& conf,
	const components::ComponentContext& ctx)
	: server::handlers::HttpHandlerJsonBase{conf, ctx}
	, _s{ctx.FindComponent<Service>()}
{}

formats::json::Value UserList::HandleRequestJsonThrow(
	const server::http::HttpRequest& req,
	const formats::json::Value& body,
	server::request::RequestContext&) const
{
	formats::json::ValueBuilder res;

	try {
		const auto sessionId = req.GetHeader(headers::kSessionId);
		if (sessionId.empty())
			throw errors::Unauthorized401{};
	
		const auto paging = parsePaging(req);
		std::string search = "";
		if (req.HasArg("search"))
			search = req.GetArg("search");

		res = _s.GetUserInfoList(search, sessionId, paging.start, paging.limit);
	} catch(...) {
		return errors::CatchIt(req);
	}

	return res.ExtractValue();
}

} // namespace svetit::auth::handlers