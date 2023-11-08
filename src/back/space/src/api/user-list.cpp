#include "user-list.hpp"
#include "../service/service.hpp"
#include "../../../shared/headers.hpp"

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

	const auto& userId = req.GetHeader(headers::kUserId);
	if (userId.empty()) {
		res["err"] = "Access denied";
		req.SetResponseStatus(server::http::HttpStatus::kUnauthorized);
		return res.ExtractValue();
	}

	const auto& start = req.GetArg("start");
	const auto& limit = req.GetArg("limit");
	const auto& spaceId = req.GetArg("spaceId");

	int iStart;
	int iLimit;

	if (!start.empty() && !limit.empty() && !spaceId.empty()){
		try {
			iStart = boost::lexical_cast<int>(start);
		} catch(const std::exception& e) {
			req.SetResponseStatus(server::http::HttpStatus::kBadRequest);
			res["err"] = "Wrong start query param";
			return res.ExtractValue();
		}

		try {
			iLimit = boost::lexical_cast<int>(limit);
		} catch(const std::exception& e) {
			req.SetResponseStatus(server::http::HttpStatus::kBadRequest);
			res["err"] = "Wrong limit query param";
			return res.ExtractValue();
		}

		if (iStart < 0) {
			req.SetResponseStatus(server::http::HttpStatus::kBadRequest);
			res["err"] = "Start param must be valid";
			return res.ExtractValue();
		}

		if (iLimit <= 0) {
			req.SetResponseStatus(server::http::HttpStatus::kBadRequest);
			res["err"] = "Limit param must be valid";
			return res.ExtractValue();
		}
	} else {
		req.SetResponseStatus(server::http::HttpStatus::kBadRequest);
		res["err"] = "Params must be set";
		return res.ExtractValue();
	}

	try {
		res["list"] = _s.GetUserList(userId, spaceId, iStart, iLimit);
		res["total"] = _s.GetUserCount(userId, spaceId);
	} catch(errors::BadRequest& e) {
		req.SetResponseStatus(server::http::HttpStatus::kBadRequest);
		res["err"] = e.what();
		return res.ExtractValue();
	} catch(const std::exception& e) {
		LOG_WARNING() << "Fail to get spaces list: " << e.what();
		res["err"] = "Fail to get spaces list";
		req.SetResponseStatus(server::http::HttpStatus::kInternalServerError);
	}

	return res.ExtractValue();
}

} // namespace svetit::space::handlers