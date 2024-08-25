#include "list.hpp"
#include "../model/node_serialize.hpp"
#include "../service/service.hpp"
#include <shared/headers.hpp>
#include <shared/errors.hpp>
#include <shared/errors_catchit.hpp>
#include <shared/paging.hpp>
#include <shared/paging_serialize.hpp>
#include <shared/schemas.hpp>
#include <shared/parse/uuid.hpp>

#include <boost/lexical_cast.hpp>

namespace svetit::node::handlers {

List::List(
	const components::ComponentConfig& conf,
	const components::ComponentContext& ctx)
	: server::handlers::HttpHandlerJsonBase{conf, ctx}
	, _s{ctx.FindComponent<Service>()}
	, _mapHttpMethodToSchema{LoadSchemas(kName, _s.GetJSONSchemasPath())}
{}

formats::json::Value List::HandleRequestJsonThrow(
	const server::http::HttpRequest& req,
	const formats::json::Value& body,
	server::request::RequestContext&) const
{
	formats::json::ValueBuilder res;

	try {
		const auto params = ValidateRequest(_mapHttpMethodToSchema, req, body);
		const auto userId = params[headers::kUserId].As<std::string>();
		const auto spaceId = params[headers::kSpaceId].As<boost::uuids::uuid>();

		const auto paging = parsePaging(params);

		const auto isAdmin = boost::lexical_cast<bool>(params[headers::kSpaceIsAdmin].As<std::string>());
		if (isAdmin)
			res = _s.Repo().Node().GetList(paging.start, paging.limit, spaceId);
		else
			res = _s.Repo().GetNodeList(paging.start, paging.limit, spaceId, userId);
	} catch(...) {
		return errors::CatchIt(req);
	}

	return res.ExtractValue();
}

} // namespace svetit::node::handlers
