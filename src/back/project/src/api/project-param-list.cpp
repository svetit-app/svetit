#include "project-param-list.hpp"
#include "../service/service.hpp"
#include "../model/project_param_serialize.hpp"
#include <shared/errors.hpp>
#include <shared/errors_catchit.hpp>
// #include <shared/headers.hpp>
#include <shared/paging.hpp>
#include <shared/paging_serialize.hpp>

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
	// use try/catch blocks
	formats::json::ValueBuilder res;

	auto paging = parsePaging(req);
	const auto keepDeletedStr = req.GetArg("keepDeleted");
	// make common shared func to get such param
	bool keepDeleted = (strcasecmp("true",keepDeletedStr.c_str()) == 0);
	// if (_s.IsListLimit(paging.limit))
	// 	throw errors::BadRequest400("Too big limit param");

	// also keepDeleted param needed
	res = _s.GetProjectParamList(paging.start, paging.limit, keepDeleted);

	return res.ExtractValue();
}

} // namespace svetit::project::handlers
