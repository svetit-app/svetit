#include "project.hpp"
#include "../service/service.hpp"
#include "../model/project.hpp"
#include "../model/project_serialize.hpp"
#include <shared/errors.hpp>
#include <shared/errors_catchit.hpp>
#include <shared/parse/request.hpp>
#include <shared/schemas.hpp>

#include <userver/fs/blocking/read.hpp>

namespace svetit::project::handlers {

Project::Project(
	const components::ComponentConfig& conf,
	const components::ComponentContext& ctx)
	: server::handlers::HttpHandlerJsonBase{conf, ctx}
	, _s{ctx.FindComponent<Service>()}
	, _mapHttpMethodToSchema{}
{
	LoadSchemas(std::string(kName), _s.GetJSONSchemasPath(), server::http::HttpMethod::kGet, true, false, _mapHttpMethodToSchema);

	LoadSchemas(std::string(kName), _s.GetJSONSchemasPath(), server::http::HttpMethod::kPost, true, true, _mapHttpMethodToSchema);

	LoadSchemas(std::string(kName), _s.GetJSONSchemasPath(), server::http::HttpMethod::kPatch, true, true, _mapHttpMethodToSchema);

	LoadSchemas(std::string(kName), _s.GetJSONSchemasPath(), server::http::HttpMethod::kDelete, true, false, _mapHttpMethodToSchema);
}

formats::json::Value Project::HandleRequestJsonThrow(
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

formats::json::Value Project::Get(
	const server::http::HttpRequest& req,
	formats::json::ValueBuilder& res) const
{
	ValidateRequest(req, _mapHttpMethodToSchema);

	if (req.HasArg("id")) {
		const auto id = parseUUID(req, "id");
		res = _s.GetProjectById(id);
	} else if (req.HasArg("key")) {
		const auto key = req.GetArg("key");
		res = _s.GetProjectByKey(key);
	}

	return res.ExtractValue();
}

formats::json::Value Project::Post(
	const server::http::HttpRequest& req,
	const formats::json::Value& body,
	formats::json::ValueBuilder& res) const
{
	ValidateRequest(req, _mapHttpMethodToSchema);
	ValidateBody(_mapHttpMethodToSchema, req.GetMethod(), body);
	
	const auto project = body.As<model::Project>();

	_s.CreateProject(project);

	req.SetResponseStatus(server::http::HttpStatus::kCreated);
	return res.ExtractValue();
}

formats::json::Value Project::Patch(
	const server::http::HttpRequest& req,
	const formats::json::Value& body,
	formats::json::ValueBuilder& res) const
{
	ValidateRequest(req, _mapHttpMethodToSchema);
	ValidateBody(_mapHttpMethodToSchema, req.GetMethod(), body);

	const auto project = body.As<model::Project>();

	_s.UpdateProject(project);

	return res.ExtractValue();
}

formats::json::Value Project::Delete(
	const server::http::HttpRequest& req,
	formats::json::ValueBuilder& res) const
{
	ValidateRequest(req, _mapHttpMethodToSchema);

	const auto id = parseUUID(req, "id");

	_s.DeleteProject(id);

	return res.ExtractValue();
}

} // namespace svetit::project::handlers