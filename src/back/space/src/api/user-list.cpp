#include "user-list.hpp"
#include "../model/user_serialize.hpp"
#include "../service/service.hpp"
#include <shared/headers.hpp>
#include <shared/errors.hpp>
#include <shared/errors_catchit.hpp>
#include <shared/parse/request.hpp>
#include <shared/paging_serialize.hpp>

namespace svetit::space::handlers {

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
		const auto userId = req.GetHeader(headers::kUserId);
		if (userId.empty())
			throw errors::Unauthorized401{};

		const auto spaceId = parseUUID(req, "spaceId");
		const auto paging = parsePaging(req);
		if (_s.IsListLimit(paging.limit))
			throw errors::BadRequest400("Too big limit param");
		res = _s.GetUserList(userId, spaceId, paging.start, paging.limit);
	} catch(...) {
		return errors::CatchIt(req);
	}

	return res.ExtractValue();
}

} // namespace svetit::space::handlers
