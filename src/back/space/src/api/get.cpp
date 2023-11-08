#include "get.hpp"
#include "../service/service.hpp"
#include "../../../shared/headers.hpp"

namespace svetit::space::handlers {

Get::Get(
	const components::ComponentConfig& conf,
	const components::ComponentContext& ctx)
	: server::handlers::HttpHandlerJsonBase{conf, ctx}
	, _s{ctx.FindComponent<Service>()}
{}

formats::json::Value Get::HandleRequestJsonThrow(
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

	const auto& id = req.GetArg("id");
	const auto& key = req.GetArg("key");
	const auto& link = req.GetArg("link");

	if (id.empty() && key.empty() && link.empty()) {
		req.SetResponseStatus(server::http::HttpStatus::kBadRequest);
		res["err"] = "No arguments";
		return res.ExtractValue();
	}

	if (
		(!id.empty() && (!key.empty() || !link.empty()))
		|| (!key.empty() && (!id.empty() || !link.empty()))
		|| (!link.empty() && (!id.empty() || !key.empty()))
	) {
		req.SetResponseStatus(server::http::HttpStatus::kBadRequest);
		res["err"] = "Only one argument should be set";
		return res.ExtractValue();
	}

	int method;

	if (!id.empty()) {
		if (!_s.ValidateUUID(id)) {
			req.SetResponseStatus(server::http::HttpStatus::kBadRequest);
			res["err"] = "Id param must be valid";
			return res.ExtractValue();
		}
		method = 1;
	} else if (!key.empty()) {
		if (!_s.CheckKeyByRegex(key)) {
			req.SetResponseStatus(server::http::HttpStatus::kBadRequest);
			res["err"] = "Key must be valid";
			return res.ExtractValue();
		}
		method = 2;
	} else if (!link.empty()) {
		if (!_s.ValidateUUID(link)) {
			req.SetResponseStatus(server::http::HttpStatus::kBadRequest);
			res["err"] = "Link param must be valid";
			return res.ExtractValue();
		}
		method = 3;
	}

	model::Space space = {};
	bool found = false;

	try {
		switch (method) {
			case 1:
				space = _s.GetById(id, found, userId);
				break;
			case 2:
				space = _s.GetByKey(key, found, userId);
				break;
			case 3:
				space = _s.GetByLink(link, found);
				break;
		}
	} catch(const std::exception& e) {
		LOG_WARNING() << "Fail to get space: " << e.what();
		res["err"] = "Fail to get space";
		req.SetResponseStatus(server::http::HttpStatus::kInternalServerError);
	}

	if (found) {
		res = space;
	} else {
		req.SetResponseStatus(server::http::HttpStatus::kNotFound);
	}

	return res.ExtractValue();
}

} // namespace svetit::space::handlers