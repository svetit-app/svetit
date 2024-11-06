#include "node-project.hpp"
#include "../model/node_project_serialize.hpp"
#include "../service/service.hpp"
#include <svetit/headers.hpp>
#include <svetit/errors.hpp>
#include <svetit/errors_catchit.hpp>
#include <svetit/paging.hpp>
#include <svetit/parse/request.hpp>
#include <svetit/schemas.hpp>
#include <svetit/parse/uuid.hpp>

namespace svetit::node::handlers {

NodeProject::NodeProject(
	const components::ComponentConfig& conf,
	const components::ComponentContext& ctx)
	: server::handlers::HttpHandlerJsonBase{conf, ctx}
	, _s{ctx.FindComponent<Service>()}
	, _mapHttpMethodToSchema{LoadSchemas(kName, _s.GetJSONSchemasPath())}
{}

formats::json::Value NodeProject::HandleRequestJsonThrow(
	const server::http::HttpRequest& req,
	const formats::json::Value& body,
	server::request::RequestContext&) const
{
	formats::json::ValueBuilder res;

	try {
		const auto params = ValidateRequest(_mapHttpMethodToSchema, req, body);

		switch (req.GetMethod()) {
			case server::http::HttpMethod::kGet:
				return Get(res, params);
			case server::http::HttpMethod::kPost:
				return Post(req, body, res, params);
			case server::http::HttpMethod::kDelete:
				return Delete(req, res, params);
			default:
				throw std::runtime_error("Unsupported");
				break;
		}
	} catch(...) {
		return errors::CatchIt(req);
	}

	return res.ExtractValue();
}

formats::json::Value NodeProject::Get(
	formats::json::ValueBuilder& res,
	const formats::json::Value& params) const
{
	const auto userId = params[headers::kUserId].As<std::string>();
	const auto nodeId = params["nodeId"].As<boost::uuids::uuid>();
	const auto projectId = params["projectId"].As<boost::uuids::uuid>();
	res = _s.GetNodeProject(nodeId, projectId, userId);
	return res.ExtractValue();
}

formats::json::Value NodeProject::Delete(
	const server::http::HttpRequest& req,
	formats::json::ValueBuilder& res,
	const formats::json::Value& params) const
{
	const auto userId = params[headers::kUserId].As<std::string>();

	const auto nodeId = params["nodeId"].As<boost::uuids::uuid>();
	const auto projectId = params["projectId"].As<boost::uuids::uuid>();

	_s.DeleteNodeProject(nodeId, projectId, userId);

	req.SetResponseStatus(server::http::HttpStatus::kNoContent);
	return res.ExtractValue();
}

formats::json::Value NodeProject::Post(
	const server::http::HttpRequest& req,
	const formats::json::Value& body,
	formats::json::ValueBuilder& res,
	const formats::json::Value& params) const
{
	const auto userId = params[headers::kUserId].As<std::string>();

	auto nodeProject = body.As<model::NodeProject>();

	_s.CreateNodeProject(nodeProject, userId);

	req.SetResponseStatus(server::http::HttpStatus::kCreated);
	return res.ExtractValue();
}

} // namespace svetit::node::handlers
