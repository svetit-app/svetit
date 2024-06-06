#include "info.hpp"
#include "../model/service_info_serialize.hpp"
#include "../model/space_service_info.hpp"
#include "../service/service.hpp"
#include <shared/errors.hpp>
#include <shared/errors_catchit.hpp>
#include <shared/headers.hpp>
#include <shared/schemas.hpp>

namespace svetit::space::handlers {

Info::Info(
	const components::ComponentConfig& conf,
	const components::ComponentContext& ctx)
	: server::handlers::HttpHandlerJsonBase{conf, ctx}
	, _s{ctx.FindComponent<Service>()}
	, _mapHttpMethodToSchema{LoadSchemas(kName, _s.GetJSONSchemasPath())}
{}

formats::json::Value Info::HandleRequestJsonThrow(
	const server::http::HttpRequest& req,
	const formats::json::Value& body,
	server::request::RequestContext&) const
{
	formats::json::ValueBuilder res;

	try {
		const auto params = ValidateRequest(_mapHttpMethodToSchema, req, body);
		const auto userId = params[headers::kUserId].As<std::string>();

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
