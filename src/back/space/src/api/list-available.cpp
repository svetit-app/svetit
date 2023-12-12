#include "list-available.hpp"
#include "../service/service.hpp"
#include "../../../shared/headers.hpp"
#include "../../../shared/errors.hpp"
#include "../../../shared/paging.hpp"
#include "../../../shared/paging_serialize.hpp"
#include "../model/space_serialize.hpp"

namespace svetit::space::handlers {

ListAvailable::ListAvailable(
	const components::ComponentConfig& conf,
	const components::ComponentContext& ctx)
	: server::handlers::HttpHandlerJsonBase{conf, ctx}
	, _s{ctx.FindComponent<Service>()}
{}

formats::json::Value ListAvailable::HandleRequestJsonThrow(
	const server::http::HttpRequest& req,
	const formats::json::Value& body,
	server::request::RequestContext&) const
{
	formats::json::ValueBuilder res;

	try {
		const auto userId = req.GetHeader(headers::kUserId);
		if (userId.empty())
			throw errors::Unauthorized{};

		const auto paging = parsePaging(req);
		if (_s.IsListLimit(paging.limit))
			throw errors::BadRequest("Too big limit param");

		std::string spaceName;
		if (req.HasArg("spaceName")) {
			spaceName = req.GetArg("spaceName");
			if (spaceName.empty())
				throw errors::BadRequest("SpaceName param shouldn't be empty");
			res = _s.GetAvailableListBySpaceName(spaceName, userId, paging.start, paging.limit);
			return res.ExtractValue();
		}

		res = _s.GetAvailableList(userId, paging.start, paging.limit);
	} catch(const errors::Unauthorized& e) {
		req.SetResponseStatus(server::http::HttpStatus::kUnauthorized);
		res["err"] = e.what();
		return res.ExtractValue();
	} catch(const errors::BadRequest& e) {
		res["err"] = e.what();
		req.SetResponseStatus(server::http::HttpStatus::kBadRequest);
		return res.ExtractValue();
	} catch(const std::exception& e) {
		LOG_WARNING() << "Fail to get spaces available list: " << e.what();
		res["err"] = "Fail to get spaces available list";
		req.SetResponseStatus(server::http::HttpStatus::kInternalServerError);
	}

	return res.ExtractValue();
}

} // namespace svetit::space::handlers