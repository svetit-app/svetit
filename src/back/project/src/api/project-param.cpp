#include "project-param.hpp"
#include "../service/service.hpp"
#include "../service/service.hpp"
#include "../model/project_param.hpp"
#include "../model/project_param_serialize.hpp"
#include <shared/errors.hpp>
#include <shared/errors_catchit.hpp>
#include <shared/parse/request.hpp>

namespace svetit::project::handlers {

ProjectParam::ProjectParam(
	const components::ComponentConfig& conf,
	const components::ComponentContext& ctx)
	: server::handlers::HttpHandlerJsonBase{conf, ctx}
	, _s{ctx.FindComponent<Service>()}
{}

formats::json::Value ProjectParam::HandleRequestJsonThrow(
	const server::http::HttpRequest& req,
	const formats::json::Value& body,
	server::request::RequestContext&) const
{
	formats::json::ValueBuilder res;

	try {
		switch (req.GetMethod()) {
		case server::http::HttpMethod::kGet:
			return Get(req, res);
		case server::http::HttpMethod::kPost:
			return Post(req, body, res);
		case server::http::HttpMethod::kPatch:
			return Patch(req, body, res);
		case server::http::HttpMethod::kDelete:
			return Delete(req, res);
		default:
			throw std::runtime_error("Unsupported");
			break;
		}
	} catch(...) {
		return errors::CatchIt(req);
	}

	return res.ExtractValue();
}

formats::json::Value ProjectParam::Get(
	const server::http::HttpRequest& req,
	formats::json::ValueBuilder& res) const
{
	const auto projectId = parseUUID(req, "projectId");
	const auto paramId = parsePositiveInt(req, "paramId");
	res = _s.GetProjectParam(projectId, paramId);

	return res.ExtractValue();
}

formats::json::Value ProjectParam::Post(
	const server::http::HttpRequest& req,
	const formats::json::Value& body,
	formats::json::ValueBuilder& res) const
{
	const auto projectParam = body.As<model::ProjectParam>();

	_s.CreateProjectParam(projectParam);

	req.SetResponseStatus(server::http::HttpStatus::kCreated);
	return res.ExtractValue();
}

formats::json::Value ProjectParam::Patch(
	const server::http::HttpRequest& req,
	const formats::json::Value& body,
	formats::json::ValueBuilder& res) const
{
	const auto projectParam = body.As<model::ProjectParam>();

	_s.UpdateProjectParam(projectParam);

	return res.ExtractValue();
}

formats::json::Value ProjectParam::Delete(
	const server::http::HttpRequest& req,
	formats::json::ValueBuilder& res) const
{
	const auto projectId = parseUUID(req, "projectId");
	const auto paramId = parsePositiveInt(req, "paramId");
	_s.DeleteProjectParam(projectId, paramId);

	return res.ExtractValue();
}

} // namespace svetit::project::handlers
