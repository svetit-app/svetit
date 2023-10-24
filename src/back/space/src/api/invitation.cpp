#include "invitation.hpp"
#include "../service/service.hpp"

namespace svetit::space::handlers {

Invitation::Invitation(
	const components::ComponentConfig& conf,
	const components::ComponentContext& ctx)
	: server::handlers::HttpHandlerJsonBase{conf, ctx}
	, _s{ctx.FindComponent<Service>()}
{}

formats::json::Value Invitation::HandleRequestJsonThrow(
	const server::http::HttpRequest& req,
	const formats::json::Value& body,
	server::request::RequestContext&) const
{
	formats::json::ValueBuilder res;

	try {
		res["items"] = _s.GetInvitationList();
	}
	catch(const std::exception& e) {
		LOG_WARNING() << " Fail to get invitations list: " << e.what();
		req.SetResponseStatus(server::http::HttpStatus::kInternalServerError);
	}

	return res.ExtractValue();
}

} // namespace svetit::space::handlers
