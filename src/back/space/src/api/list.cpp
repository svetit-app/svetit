#include "list.hpp"
#include "../model/space_serialize.hpp"
#include "../service/service.hpp"
#include <svetit/headers.hpp>
#include <svetit/errors.hpp>
#include <svetit/errors_catchit.hpp>
#include <svetit/paging.hpp>
#include <svetit/paging_serialize.hpp>
#include <svetit/schemas.hpp>

namespace svetit::space::handlers {

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

		const auto paging = parsePaging(params);
		res = _s.GetList(userId, paging.start, paging.limit);
	} catch(...) {
		return errors::CatchIt(req);
	}

	return res.ExtractValue();
}

} // namespace svetit::space::handlers
