#include "info.hpp"
#include "../service/service.hpp"
#include "../../../shared/headers.hpp"

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

	const auto& userId = req.GetHeader(headers::kUserId);
	if (userId.empty()) {
		res["err"] = "Access denied";
		req.SetResponseStatus(server::http::HttpStatus::kUnauthorized);
		return res.ExtractValue();
	}

	bool resCanCreate;
	int resInvitationAvailable;

	try {
		res = model::SpaceServiceInfo{
			.canCreate = _s.isCanCreate(),
			.invitationSize = _s.CountInvitationAvailable(userId)
		};
	}
	catch(const std::exception& e) {
		LOG_WARNING() << "Fail to get spaces info: " << e.what();
		res["err"] = "Fail to get spaces info";
		req.SetResponseStatus(server::http::HttpStatus::kInternalServerError);
		return res.ExtractValue();
	}

	return res.ExtractValue();
}

} // namespace svetit::space::handlers