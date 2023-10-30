#include "create.hpp"
#include "../service/service.hpp"
#include "../../../shared/headers.hpp"
#include <regex>

namespace svetit::space::handlers {

Create::Create(
	const components::ComponentConfig& conf,
	const components::ComponentContext& ctx)
	: server::handlers::HttpHandlerJsonBase{conf, ctx}
	, _s{ctx.FindComponent<Service>()}
{}

formats::json::Value Create::HandleRequestJsonThrow(
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

	if (!body.HasMember("key")) {
		LOG_WARNING() << "No key param in body";
		req.SetResponseStatus(server::http::HttpStatus::kBadRequest);
		res["err"] = "No key param in body";
		return res.ExtractValue();
	}

	if (!body.HasMember("name")) {
		LOG_WARNING() << "No name param in body";
		req.SetResponseStatus(server::http::HttpStatus::kBadRequest);
		res["err"] = "No name param in body";
		return res.ExtractValue();
	}

	if (!body.HasMember("requestsAllowed")) {
		LOG_WARNING() << "No requestsAllowed param in body";
		req.SetResponseStatus(server::http::HttpStatus::kBadRequest);
		res["err"] = "No requestsAllowed param in body";
		return res.ExtractValue();
	}

	auto name = (body["name"]).ConvertTo<std::string>();
	auto key = (body["key"]).ConvertTo<std::string>();
	auto requestsAllowed = (body["requestsAllowed"]).ConvertTo<bool>();

	boost::trim(name);
	boost::trim(key);

	if (!_s.CheckKeyByRegex(key)) {
		LOG_WARNING() << "Invalid key (regex)";
		req.SetResponseStatus(server::http::HttpStatus::kBadRequest);
		res["err"] = "Invalid key (regex)";
		return res.ExtractValue();
	}

	if (_s.isSpaceExistsByKey(key)) {
		LOG_WARNING() << "Invalid key (in use)";
		req.SetResponseStatus(server::http::HttpStatus::kConflict);
		res["err"] = "Invalid key (in use)";
		return res.ExtractValue();
	}

	try {
		std::string msg;
		if (_s.Create(name, key, requestsAllowed, userId, msg)) {
			req.SetResponseStatus(server::http::HttpStatus::kCreated);
		} else {
			req.SetResponseStatus(server::http::HttpStatus::kBadRequest);
			res["err"] = msg;
			LOG_WARNING() << msg;
		}
	}
	catch(const std::exception& e) {
		LOG_WARNING() << "Fail to create space: " << e.what();
		res["err"] = "Fail to create space";
		req.SetResponseStatus(server::http::HttpStatus::kInternalServerError);
	}

	return res.ExtractValue();
}

} // namespace svetit::space::handlers
