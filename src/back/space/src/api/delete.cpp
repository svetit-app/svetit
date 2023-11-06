#include "delete.hpp"
#include "../service/service.hpp"
#include "../../../shared/headers.hpp"

namespace svetit::space::handlers {

Delete::Delete(
	const components::ComponentConfig& conf,
	const components::ComponentContext& ctx)
	: server::handlers::HttpHandlerJsonBase{conf, ctx}
	, _s{ctx.FindComponent<Service>()}
{}

formats::json::Value Delete::HandleRequestJsonThrow(
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

	const auto& id = req.GetArg("id");

	if (id.empty()) {
		LOG_WARNING() << "Param id must be set";
		req.SetResponseStatus(server::http::HttpStatus::kBadRequest);
		res["err"] = "Param id must be set";
		return res.ExtractValue();
	}

   	if (!_s.ValidateUUID(id)) {
	 	LOG_WARNING() << "Id param must be uuid, id=" << id;
		req.SetResponseStatus(server::http::HttpStatus::kBadRequest);
		res["err"] = "Id param must be uuid";
		return res.ExtractValue();
	}

	try {
		if (!_s.Delete(id, userId))
			req.SetResponseStatus(server::http::HttpStatus::kNotFound);
	}
	catch(const std::exception& e) {
		LOG_WARNING() << "Fail to delete space: " << e.what();
		res["err"] = "Fail to delete space";
		req.SetResponseStatus(server::http::HttpStatus::kInternalServerError);
	}

	return res.ExtractValue();
}

} // namespace svetit::space::handlers