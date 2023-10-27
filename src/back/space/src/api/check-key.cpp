#include "check-key.hpp"
#include "../service/service.hpp"

namespace svetit::space::handlers {

CheckKey::CheckKey(
	const components::ComponentConfig& conf,
	const components::ComponentContext& ctx)
	: server::handlers::HttpHandlerJsonBase{conf, ctx}
	, _s{ctx.FindComponent<Service>()}
{}

formats::json::Value CheckKey::HandleRequestJsonThrow(
	const server::http::HttpRequest& req,
	const formats::json::Value& body,
	server::request::RequestContext&) const
{
	formats::json::ValueBuilder res;

	if (!body.HasMember("key")) {
		LOG_WARNING() << "No key param in body";
		req.SetResponseStatus(server::http::HttpStatus::kBadRequest);
		res["err"] = "No key param in body";
		return res.ExtractValue();
	}

	const auto key = body["key"];

	try {
		res["result"] = _s.isSpaceExistsByKey(key.ConvertTo<std::string>());
	}
	catch(const std::exception& e) {
		LOG_WARNING() << "Fail to check space existance by key: " << e.what();
		res["err"] = "Fail to check space existance by key";
		req.SetResponseStatus(server::http::HttpStatus::kInternalServerError);
	}

	return res.ExtractValue();
}

} // namespace svetit::space::handlers
