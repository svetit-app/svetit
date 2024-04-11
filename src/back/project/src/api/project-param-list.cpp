#include "project-param-list.hpp"
#include "../service/service.hpp"
#include "../model/project_param_serialize.hpp"
#include <shared/errors.hpp>
#include <shared/errors_catchit.hpp>
#include <shared/paging.hpp>
#include <shared/paging_serialize.hpp>
#include <shared/parse/request.hpp>

namespace svetit::project::handlers {

ProjectParamList::ProjectParamList(
	const components::ComponentConfig& conf,
	const components::ComponentContext& ctx)
	: server::handlers::HttpHandlerJsonBase{conf, ctx}
	, _s{ctx.FindComponent<Service>()}
{}

formats::json::Value ProjectParamList::HandleRequestJsonThrow(
	const server::http::HttpRequest& req,
	const formats::json::Value& body,
	server::request::RequestContext&) const
{
	formats::json::ValueBuilder res;

	try {
		auto paging = parsePaging(req);
		const auto keepDeleted = parseBool(req, "keepDeleted");
		res = _s.GetProjectParamList(paging.start, paging.limit, keepDeleted);
	} catch(...) {
		return errors::CatchIt(req);
	}

	return res.ExtractValue();
}

} // namespace svetit::project::handlers
