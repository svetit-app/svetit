#include "role-list.hpp"
#include "../model/role_serialize.hpp"
#include "../service/service.hpp"
#include "../repo/repository.hpp"
#include <shared/headers.hpp>
#include <shared/errors.hpp>
#include <shared/errors_catchit.hpp>
#include <shared/paging.hpp>
#include <shared/paging_serialize.hpp>
#include <shared/schemas.hpp>
#include <shared/parse/uuid.hpp>

namespace svetit::space::handlers {

RoleList::RoleList(
	const components::ComponentConfig& conf,
	const components::ComponentContext& ctx)
	: server::handlers::HttpHandlerJsonBase{conf, ctx}
	, _s{ctx.FindComponent<Service>()}
	, _mapHttpMethodToSchema{LoadSchemas(kName, _s.GetJSONSchemasPath())}
{}

formats::json::Value RoleList::HandleRequestJsonThrow(
	const server::http::HttpRequest& req,
	const formats::json::Value& body,
	server::request::RequestContext&) const
{
	formats::json::ValueBuilder res;

	try {
		const auto params = ValidateRequest(_mapHttpMethodToSchema, req, body);
		const auto spaceId = params[headers::kSpaceId].As<boost::uuids::uuid>();
		const auto paging = parsePaging(params);

		res = _s.Repo().Role().GetList(paging.start, paging.limit, spaceId);
		return res.ExtractValue();
	} catch(...) {
		return errors::CatchIt(req);
	}

	return res.ExtractValue();
}

} // namespace svetit::space::handlers
