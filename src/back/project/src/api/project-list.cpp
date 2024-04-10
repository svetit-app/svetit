#include "project-list.hpp"
#include "../service/service.hpp"
#include "../model/project_serialize.hpp"
#include <shared/errors.hpp>
#include <shared/errors_catchit.hpp>
// #include <shared/headers.hpp>
#include <shared/paging.hpp>
#include <shared/paging_serialize.hpp>

namespace svetit::project::handlers {

ProjectList::ProjectList(
	const components::ComponentConfig& conf,
	const components::ComponentContext& ctx)
	: server::handlers::HttpHandlerJsonBase{conf, ctx}
	, _s{ctx.FindComponent<Service>()}
{}

formats::json::Value ProjectList::HandleRequestJsonThrow(
	const server::http::HttpRequest& req,
	const formats::json::Value& body,
	server::request::RequestContext&) const
{
	formats::json::ValueBuilder res;

	try {
		auto paging = parsePaging(req);

		if (_s.IsListLimit(paging.limit))
			throw errors::BadRequest400("Too big limit param");

		res = _s.GetProjectList(paging.start, paging.limit);
	} catch(...) {
		return errors::CatchIt(req);
	}

	return res.ExtractValue();
}

} // namespace svetit::project::handlers
