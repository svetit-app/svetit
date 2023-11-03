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

	// todo - need to validate that this is valid uuid? need to check that user exists?
	const auto& userId = req.GetHeader(headers::kUserId);
	if (userId.empty()) {
		res["err"] = "Empty userId header";
		req.SetResponseStatus(server::http::HttpStatus::kUnauthorized);
		return res.ExtractValue();
	}

	bool resCanCreate;
	int resInvitationAvailable;

	try {
		resCanCreate = _s.isCanCreate();
		resInvitationAvailable = _s.CountInvitationAvailable(utils::BoostUuidFromString(userId));
	}
	catch(const std::exception& e) {
		LOG_WARNING() << "Fail to get spaces info: " << e.what();
		res["err"] = "Fail to get spaces info";
		req.SetResponseStatus(server::http::HttpStatus::kInternalServerError);
		return res.ExtractValue();
	}

	res["canCreate"] = resCanCreate;
	res["invitationSize"] = resInvitationAvailable;

	return res.ExtractValue();
}

} // namespace svetit::space::handlers