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
	// need to think about refactoring here - move it to separate method
	// get
	auto jsonSchemaParamsPath = _s.GetJSONSchemasPath() + std::string(kName) + "-get.json";
	auto jsonSchemaParams = fs::blocking::ReadFileContents(jsonSchemaParamsPath);
	SchemasForMethod schemas;
	schemas.params = jsonSchemaParams;
	schemas.body = "";
	_mapHttpMethodToSchema.insert({server::http::HttpMethod::kGet, schemas});

	// need to think about refactoring here - move it to separate method
	// post
	jsonSchemaParamsPath = _s.GetJSONSchemasPath() + std::string(kName) + "-post.json";
	jsonSchemaParams = fs::blocking::ReadFileContents(jsonSchemaParamsPath);
	schemas.params = jsonSchemaParams;
	schemas.body = GetBodySchemaFromRequestBody(jsonSchemaParams, _s.GetJSONSchemasPath());
	_mapHttpMethodToSchema.insert({server::http::HttpMethod::kPost, schemas});
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
	// refactoring needed - move it to separate method/func - custom params validator
	auto jsonSchemasForMethod = _mapHttpMethodToSchema.at(req.GetMethod());
	auto schemaDocumentParams = formats::json::FromString(jsonSchemasForMethod.params);

	std::string jsonDocumentStr = GenerateJsonDocument(schemaDocumentParams, req);
	LOG_WARNING() << "JsonDocumentStr by request params: " << jsonDocumentStr;
	formats::json::Value jsonDocument;

	try {
		jsonDocument = formats::json::FromString(jsonDocumentStr);
		formats::json::Schema schema(schemaDocumentParams);
		auto result = formats::json::Validate(jsonDocument, schema);
		LOG_WARNING() << "Validation result: " << result;
		if (!result)
			throw errors::BadRequest400("wrong params");
	} catch (formats::json::ParseException& e) {
		throw errors::BadRequest400("wrong params");
	}
	//

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
	// refactoign needed - move it to separate func\method - custom request params and headers validation
	auto jsonSchemasForMethod = _mapHttpMethodToSchema.at(req.GetMethod());
	auto schemaDocumentParams = formats::json::FromString(jsonSchemasForMethod.params);

	std::string jsonDocumentStr = GenerateJsonDocument(schemaDocumentParams, req);
	LOG_WARNING() << "JsonDocumentStr by request params: " << jsonDocumentStr;
	formats::json::Value jsonDocument;

	try {
		jsonDocument = formats::json::FromString(jsonDocumentStr);
		formats::json::Schema schema(schemaDocumentParams);
		auto result = formats::json::Validate(jsonDocument, schema);
		LOG_WARNING() << "Validation result: " << result;
		if (!result)
			throw errors::BadRequest400("wrong params");
	} catch (formats::json::ParseException& e) {
		throw errors::BadRequest400("wrong params");
	}

	// refactoring needed - move it to separate func\method - custom body validator
	jsonSchemasForMethod = _mapHttpMethodToSchema.at(req.GetMethod());
	auto schemaDocumentBody = formats::json::FromString(jsonSchemasForMethod.body);
	LOG_WARNING() << "SchemaDocumentBody: " << schemaDocumentBody;
	formats::json::Schema schema(schemaDocumentBody);
	auto result = formats::json::Validate(body, schema);
	LOG_WARNING() << "Validation result: " << result;
	if (!result)
		throw errors::BadRequest400("wrong params");
	//

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
	const auto project = body.As<model::Project>();

	_s.UpdateProject(project);

	return res.ExtractValue();
}

formats::json::Value Project::Delete(
	const server::http::HttpRequest& req,
	formats::json::ValueBuilder& res) const
{
	const auto id = parseUUID(req, "id");

	_s.DeleteProject(id);

	return res.ExtractValue();
}

} // namespace svetit::project::handlers