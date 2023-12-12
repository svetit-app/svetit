#include "info.hpp"
#include "../../../shared/errors.hpp"
#include "../../../shared/errors_catchit.hpp"
#include "../model/space_service_info.hpp"
#include "../model/service_info_serialize.hpp"
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

	try {
		const auto userId = req.GetHeader(headers::kUserId);
		if (userId.empty())
			throw errors::Unauthorized401{};

		res = model::SpaceServiceInfo{
			.canCreate = _s.isCanCreate(),
			.invitationSize = _s.CountInvitationAvailable(userId)
		};
	} catch(...) {
		return errors::CatchIt(req);
	}

	return res.ExtractValue();
}

} // namespace svetit::space::handlers