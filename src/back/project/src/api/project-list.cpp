#include "project-list.hpp"
#include "../service/service.hpp"
#include "../model/project_serialize.hpp"
#include <shared/errors.hpp>
#include <shared/errors_catchit.hpp>
#include <shared/paging.hpp>
#include <shared/paging_serialize.hpp>
#include <shared/schemas.hpp>

#include <userver/formats/json/validate.hpp>
#include <userver/fs/blocking/read.hpp>

namespace svetit::project::handlers {

ProjectList::ProjectList(
	const components::ComponentConfig& conf,
	const components::ComponentContext& ctx)
	: server::handlers::HttpHandlerJsonBase{conf, ctx}
	, _s{ctx.FindComponent<Service>()}
	, _mapHttpMethodToSchema{}
{
	const auto jsonSchemaParamsPath = _s.GetJSONSchemasPath() + std::string(kName) + "-get.json";
	const auto jsonSchemaParams = fs::blocking::ReadFileContents(jsonSchemaParamsPath);

	SchemasForMethod schemas;
	schemas.params = jsonSchemaParams;
	schemas.body = "";

	const auto jsonSchemaParamsJson = formats::json::FromString(jsonSchemaParams);
	if (jsonSchemaParamsJson.HasMember("requestBody")) {
		const auto requestBody = jsonSchemaParamsJson["requestBody"];
		const auto requestBodyPath = _s.GetJSONSchemasPath() + requestBody.As<std::string>() + ".json";
		const auto requestBodyFileContents = fs::blocking::ReadFileContents(requestBodyPath);
		schemas.body = requestBodyFileContents;
	}
	_mapHttpMethodToSchema.insert({server::http::HttpMethod::kGet, schemas});
}

formats::json::Value ProjectList::HandleRequestJsonThrow(
	const server::http::HttpRequest& req,
	const formats::json::Value& body,
	server::request::RequestContext&) const
{
	formats::json::ValueBuilder res;

	try {
		std::string kHandlerProjectListJsonRequestParams{R"(
			{
				"start": 0,
				"limit": 5
			}
		)"};

		auto jsonSchemasForMethod = _mapHttpMethodToSchema.at(req.GetMethod());
		auto schemaDocumentParams = formats::json::FromString(jsonSchemasForMethod.params);

		// need to generate jsonDocument from request params and json schema
		auto jsonDocument = formats::json::FromString(kHandlerProjectListJsonRequestParams);

		formats::json::Schema schema(schemaDocumentParams);

		// need to write exception throw if not valid
		const auto validationResult = formats::json::Validate(jsonDocument, schema);
		LOG_WARNING() << "Validation Result = " << validationResult;

		auto paging = parsePaging(req);
		res = _s.GetProjectList(paging.start, paging.limit);
	} catch(...) {
		return errors::CatchIt(req);
	}

	return res.ExtractValue();
}

} // namespace svetit::project::handlers
