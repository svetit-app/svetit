#include "schemas.hpp"
#include "errors.hpp"

#include <string>
#include <boost/algorithm/string.hpp>

#include <userver/server/handlers/http_handler_json_base.hpp>
#include <userver/utest/using_namespace_userver.hpp>
#include <userver/fs/blocking/read.hpp>

namespace svetit {

void LoadSchemas(const std::string& handlerName, const std::string& schemasFolderPath, const server::http::HttpMethod& httpMethod, bool loadParams, bool loadBody, std::map<server::http::HttpMethod, SchemasForMethod>& _mapHttpMethodToSchema) {
	std::string httpMethodStr;
	switch (httpMethod) {
		case server::http::HttpMethod::kGet:
			httpMethodStr = "get";
			break;
		case server::http::HttpMethod::kPost:
			httpMethodStr = "post";
			break;
		case server::http::HttpMethod::kPatch:
			httpMethodStr = "patch";
			break;
		case server::http::HttpMethod::kDelete:
			httpMethodStr = "delete";
			break;
		case server::http::HttpMethod::kConnect:
			httpMethodStr = "connect";
			break;
		case server::http::HttpMethod::kHead:
			httpMethodStr = "head";
			break;
		case server::http::HttpMethod::kOptions:
			httpMethodStr = "options";
			break;
		case server::http::HttpMethod::kPut:
			httpMethodStr = "put";
			break;
		default:
			throw std::runtime_error("Unsupported");
			break;
	}
	const auto jsonSchemaParamsPath = schemasFolderPath + handlerName + "-" + httpMethodStr + ".json";
	const auto jsonSchemaParams = fs::blocking::ReadFileContents(jsonSchemaParamsPath);

	SchemasForMethod schemas;
	schemas.params = "";
	schemas.body = "";
	// maybe loadParams and loadBody are not necessary
	if (loadParams)
		schemas.params = jsonSchemaParams;
	if (loadBody)
		schemas.body = GetBodySchemaFromRequestBody(jsonSchemaParams, schemasFolderPath);
	_mapHttpMethodToSchema.insert({httpMethod, schemas});
}

std::string GetBodySchemaFromRequestBody(const std::string& jsonSchemaParams, const std::string& path) {
	const auto jsonSchemaParamsJson = formats::json::FromString(jsonSchemaParams);
	const auto requestBody = jsonSchemaParamsJson["requestBody"];
	const auto requestBodyPath = path + boost::algorithm::to_lower_copy(requestBody.As<std::string>());
	const auto requestBodyFileContents = fs::blocking::ReadFileContents(requestBodyPath);
	return requestBodyFileContents;
}

std::string GenerateJsonDocument(
	const formats::json::Value& schemaDocumentParams,
	const server::http::HttpRequest& req
) {
	std::string jsonDocumentStr;
	if (schemaDocumentParams.HasMember("properties")){
		auto properties = schemaDocumentParams["properties"];
		jsonDocumentStr = "{";
		for (auto i = properties.begin(); i != properties.end(); ++i)	{
			auto param = i->GetPath().substr(strlen("properties."));
			if (req.HasArg(param) && !req.GetArg(param).empty()) {
				// maybe moving this if/else block to separate func
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
					// maybe more logs here or custom exception for debugging
					throw std::exception();
				}
				jsonDocumentStr += ",";
			} else if (req.HasHeader(param) && !req.GetHeader(param).empty()) {
				// maybe moving this if/else block to separate func
				jsonDocumentStr += "\"" + param + "\"" + ":";
				auto type = (*i)["type"].As<std::string>();
				auto value = req.GetHeader(param);
				if (type == "string") {
					jsonDocumentStr += "\"" + value + "\"";
				} else if (type == "number") {
					jsonDocumentStr += value;
				} else if (type == "integer") {
					jsonDocumentStr += value;
				} else if (type == "boolean") {
					jsonDocumentStr += value;
				} else {
					// maybe more logs here or custom exception for debugging
					throw std::exception();
				}
				jsonDocumentStr += ",";
			}
		}
		if (jsonDocumentStr.back() == ',')
			jsonDocumentStr.pop_back();
		jsonDocumentStr += "}";
		return jsonDocumentStr;
	}
}

void ValidateRequest(const server::http::HttpRequest& req,  const std::map<server::http::HttpMethod, SchemasForMethod>& map) {
	auto jsonSchemasForMethod = map.at(req.GetMethod());

	auto schemaDocumentParams = formats::json::FromString(jsonSchemasForMethod.params);

	std::string jsonDocumentStr = GenerateJsonDocument(schemaDocumentParams, req);
	LOG_WARNING() << "JsonDocumentStr by request params and headers: " << jsonDocumentStr;
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
}

} // svetit
