#include "list.hpp"
#include "../service/service.hpp"
#include "../../../shared/headers.hpp"

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
		res["err"] = "Empty userId header";
		req.SetResponseStatus(server::http::HttpStatus::kUnauthorized);
		return res.ExtractValue();
	}

	const auto& start = req.GetArg("start");
	const auto& limit = req.GetArg("limit");
	int iStart;
	int iLimit;

	if (!start.empty() && !limit.empty()){
		try {
			iStart = boost::lexical_cast<int>(start);
		} catch(const std::exception& e) {
			LOG_WARNING() << "Wrong start query param: " << e.what() << " , start=" << start;;
			req.SetResponseStatus(server::http::HttpStatus::kBadRequest);
			res["err"] = "Wrong start query param";
			return res.ExtractValue();
		}

		try {
			iLimit = boost::lexical_cast<int>(limit);
		} catch(const std::exception& e) {
			LOG_WARNING() << "Wrong limit query param: " << e.what() << " , limit=" << limit;
			req.SetResponseStatus(server::http::HttpStatus::kBadRequest);
			res["err"] = "Wrong limit query param";
			return res.ExtractValue();
		}

		if (iStart < 0) {
			LOG_WARNING() << "Start param must be more then 0, gotten=" << iStart;
			req.SetResponseStatus(server::http::HttpStatus::kBadRequest);
			res["err"] = "Start param must be more then 0";
			return res.ExtractValue();
		}

		if (iLimit <= 0) {
			LOG_WARNING() << "Limit param must be more then 0, gotten=" << iLimit;
			req.SetResponseStatus(server::http::HttpStatus::kBadRequest);
			res["err"] = "Limit param must be more then 0";
			return res.ExtractValue();
		}
	} else {
		LOG_WARNING() << "Start and Limit params must be set, gotten start=" << start << " limit=" << limit;
		req.SetResponseStatus(server::http::HttpStatus::kBadRequest);
		res["err"] = "Start and Limit params must be set";
		return res.ExtractValue();
	}

	try {
		res["list"] = _s.GetList(userId, iStart, iLimit);
		res["total"] = _s.GetCount(userId);
	}
	catch(const std::exception& e) {
		LOG_WARNING() << "Fail to get spaces list: " << e.what();
		res["err"] = "Fail to get spaces list";
		req.SetResponseStatus(server::http::HttpStatus::kInternalServerError);
	}

	return res.ExtractValue();
}

} // namespace svetit::space::handlers