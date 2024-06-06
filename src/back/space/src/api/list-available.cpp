#include "list-available.hpp"
#include "../model/space_serialize.hpp"
#include "../service/service.hpp"
#include <shared/headers.hpp>
#include <shared/errors.hpp>
#include <shared/errors_catchit.hpp>
#include <shared/paging.hpp>
#include <shared/paging_serialize.hpp>
#include <shared/schemas.hpp>

namespace svetit::space::handlers {

ListAvailable::ListAvailable(
	const components::ComponentConfig& conf,
	const components::ComponentContext& ctx)
	: server::handlers::HttpHandlerJsonBase{conf, ctx}
	, _s{ctx.FindComponent<Service>()}
	, _mapHttpMethodToSchema{LoadSchemas(kName, _s.GetJSONSchemasPath())}
{}

formats::json::Value ListAvailable::HandleRequestJsonThrow(
	const server::http::HttpRequest& req,
	const formats::json::Value& body,
	server::request::RequestContext&) const
{
	formats::json::ValueBuilder res;

	try {
		const auto userId = req.GetHeader(headers::kUserId);
		if (userId.empty())
			throw errors::Unauthorized401{};

		const auto paging = parsePaging(req);
		if (_s.IsListLimit(paging.limit))
			throw errors::BadRequest400("Too big limit param");

		std::string spaceName;
		if (req.HasArg("spaceName")) {
			spaceName = req.GetArg("spaceName");
			if (spaceName.empty())
				throw errors::BadRequest400("SpaceName param shouldn't be empty");
			res = _s.GetAvailableListBySpaceName(spaceName, userId, paging.start, paging.limit);
			return res.ExtractValue();
		}

		res = _s.GetAvailableList(userId, paging.start, paging.limit);
	} catch(...) {
		return errors::CatchIt(req);
	}

	return res.ExtractValue();
}

} // namespace svetit::space::handlers
