#include "list-available.hpp"
#include "../service/service.hpp"
#include "../../../shared/headers.hpp"
#include "../../../shared/errors.hpp"

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

	const auto& userId = req.GetHeader(headers::kUserId);
	if (userId.empty()) {
		res["err"] = "Access denied";
		req.SetResponseStatus(server::http::HttpStatus::kUnauthorized);
		return res.ExtractValue();
	}

	try {
		auto paging = paging::parsePaging(req);
		res["list"] = _s.GetAvailableList(userId, paging.start, paging.limit);
		res["total"] = _s.GetAvailableCount(userId);
	}
	catch(const errors::BadRequestException& e) {
		res["err"] = e.what();
		req.SetResponseStatus(server::http::HttpStatus::kBadRequest);
		return res.ExtractValue();
	}
	catch(const std::exception& e) {
		LOG_WARNING() << "Fail to get spaces available list: " << e.what();
		res["err"] = "Fail to get spaces available list";
		req.SetResponseStatus(server::http::HttpStatus::kInternalServerError);
	}

	return res.ExtractValue();
}

} // namespace svetit::space::handlers