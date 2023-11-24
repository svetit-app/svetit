#include "list-available.hpp"
#include "../service/service.hpp"
#include "../../../shared/headers.hpp"
#include "../../../shared/errors.hpp"
#include "../../../shared/paging.hpp"
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
		const auto list = _s.GetAvailableList(userId, paging.start, paging.limit);
		res["list"] = list.items;
		res["total"] = list.total;
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