#include "list.hpp"
#include "../service/service.hpp"
#include "../../../shared/headers.hpp"
#include "../../../shared/errors.hpp"
#include "../../../shared/paging.hpp"
#include "../../../shared/paging.сpp"
#include "../model/space_serialize.hpp"

namespace svetit::space::handlers {

List::List(
	const components::ComponentConfig& conf,
	const components::ComponentContext& ctx)
	: server::handlers::HttpHandlerJsonBase{conf, ctx}
	, _s{ctx.FindComponent<Service>()}
{}

formats::json::Value List::HandleRequestJsonThrow(
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
		const auto paging = parsePaging(req);
		res["list"] = _s.GetList(userId, paging.start, paging.limit);
		res["total"] = _s.GetCount(userId);
	} catch(const errors::BadRequest& e) {
		res["err"] = e.what();
		req.SetResponseStatus(server::http::HttpStatus::kBadRequest);
		return res.ExtractValue();
	} catch(const errors::NotFound& e) {
		res["err"] = e.what();
		req.SetResponseStatus(server::http::HttpStatus::kNotFound);
		return res.ExtractValue();
	} catch(const std::exception& e) {
		LOG_WARNING() << "Fail to get spaces list: " << e.what();
		res["err"] = "Fail to get spaces list";
		req.SetResponseStatus(server::http::HttpStatus::kInternalServerError);
	}

	return res.ExtractValue();
}

} // namespace svetit::space::handlers