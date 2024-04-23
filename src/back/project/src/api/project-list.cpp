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
		auto jsonSchemasForMethod = _mapHttpMethodToSchema.at(req.GetMethod());
		auto schemaDocumentParams = formats::json::FromString(jsonSchemasForMethod.params);

		// generating jsonDocument from parameters schema and request

		std::string jsonDocumentStr;
		if (schemaDocumentParams.HasMember("properties")){
			auto properties = schemaDocumentParams["properties"];
			jsonDocumentStr = "{";
			for (auto i = properties.begin(); i != properties.end(); ++i)	{
				auto param = i->GetPath().substr(strlen("properties."));
				if (req.HasArg(param) && !req.GetArg(param).empty()) {
					jsonDocumentStr += "\"" + param + "\"" + ":";
					auto type = (*i)["type"].As<std::string>();
					auto value = req.GetArg(param);
					if (type == "string") {
						jsonDocumentStr += "\"" + value + "\"";
					} else if (type == "number") {
						jsonDocumentStr += value;
					} else if (type == "integer") {
						jsonDocumentStr += value;
					} else if (type == "boolean") {
						jsonDocumentStr += value;
					} else {
						throw std::exception();
					}
					jsonDocumentStr += ",";
				}
			}
			if (jsonDocumentStr.back() == ',')
				jsonDocumentStr.pop_back();
			jsonDocumentStr += "}";
		}

		LOG_WARNING() << "TEST " << jsonDocumentStr;

		formats::json::Value jsonDocument;

		try {
			jsonDocument = formats::json::FromString(jsonDocumentStr);
			formats::json::Schema schema(schemaDocumentParams);
			auto result = formats::json::Validate(jsonDocument, schema);
			LOG_WARNING() << "RESULT " << result;
			if (!result)
				throw errors::BadRequest400("wrong params");
		} catch (formats::json::ParseException& e) {
			throw errors::BadRequest400("wrong params");
		}

		auto paging = parsePaging(req);
		res = _s.GetProjectList(paging.start, paging.limit);
	} catch(...) {
		return errors::CatchIt(req);
	}

	return res.ExtractValue();
}

} // namespace svetit::project::handlers
