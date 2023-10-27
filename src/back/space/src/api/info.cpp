#include "info.hpp"
#include "../service/service.hpp"

namespace svetit::space::handlers {

Info::Info(
	const components::ComponentConfig& conf,
	const components::ComponentContext& ctx)
	: server::handlers::HttpHandlerJsonBase{conf, ctx}
	, _s{ctx.FindComponent<Service>()}
{}

formats::json::Value Info::HandleRequestJsonThrow(
	const server::http::HttpRequest& req,
	const formats::json::Value& body,
	server::request::RequestContext&) const
{
	formats::json::ValueBuilder res;

	try {
		res["canCreate"] = _s.isCanCreate();
		res["invitationAvailable"] = _s.isInvitationAvailable();
	}
	catch(const std::exception& e) {
		LOG_WARNING() << "Fail to get spaces info: " << e.what();
		res["err"] = "Fail to get spaces info";
		req.SetResponseStatus(server::http::HttpStatus::kInternalServerError);
	}

	return res.ExtractValue();
}

} // namespace svetit::space::handlers
