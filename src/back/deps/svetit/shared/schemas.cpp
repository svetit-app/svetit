#include "schemas.hpp"
#include "errors.hpp"

#include <string>
#include <boost/algorithm/string.hpp>

#include <userver/server/handlers/http_handler_json_base.hpp>
#include <userver/utest/using_namespace_userver.hpp>
#include <userver/fs/blocking/read.hpp>

namespace svetit {

void LoadSchemas(
	const std::string& handlerName,
 	const std::string& schemasFolder,
	const server::http::HttpMethod& method,
	bool loadParams,
	bool loadBody,
	std::map<server::http::HttpMethod, RequestAndJsonSchema>& map
) {
	std::string methodStr;
	switch (method) {
		case server::http::HttpMethod::kGet:
			methodStr = "get";
			break;
		case server::http::HttpMethod::kPost:
			methodStr = "post";
			break;
		case server::http::HttpMethod::kPatch:
			methodStr = "patch";
			break;
		case server::http::HttpMethod::kDelete:
			methodStr = "delete";
			break;
		case server::http::HttpMethod::kConnect:
			methodStr = "connect";
			break;
		case server::http::HttpMethod::kHead:
			methodStr = "head";
			break;
		case server::http::HttpMethod::kOptions:
			methodStr = "options";
			break;
		case server::http::HttpMethod::kPut:
			methodStr = "put";
			break;
		default:
			throw std::runtime_error("Unsupported");
			break;
	}
	const auto schemaFile = schemasFolder + handlerName + "-" + methodStr + ".json";
	const auto requestSchemaStr = fs::blocking::ReadFileContents(schemaFile);

	RequestAndJsonSchema schemas;
	// maybe loadParams and loadBody are not necessary
	if (loadParams)
		schemas.request = formats::json::FromString(requestSchemaStr);
	if (loadBody)
		schemas.json = GetBodySchema(requestSchemaStr, schemasFolder);

	map.insert({method, schemas});
}

formats::json::Value GetBodySchema(
	const std::string& requestSchemaStr,
	const std::string& path
) {
	const auto requestSchema = formats::json::FromString(requestSchemaStr);
	const auto requestBodyParam = requestSchema["requestBody"];
	const auto requestBodyPath = path + boost::algorithm::to_lower_copy(requestBodyParam.As<std::string>());
	const auto requestBodyStr = fs::blocking::ReadFileContents(requestBodyPath);
	return formats::json::FromString(requestBodyStr);
}

std::string GenerateJson(
	const formats::json::Value& requestSchema,
	const server::http::HttpRequest& req
) {
	std::string json;
	if (requestSchema.HasMember("properties")){
		auto properties = requestSchema["properties"];
		json = "{";
		for (auto i = properties.begin(); i != properties.end(); ++i)	{
			auto param = i->GetPath().substr(strlen("properties."));
			if (req.HasArg(param) && !req.GetArg(param).empty()) {
				// maybe moving this if/else block to separate func
				json += "\"" + param + "\"" + ":";
				auto type = (*i)["type"].As<std::string>();
				auto value = req.GetArg(param);
				if (type == "string") {
					json += "\"" + value + "\"";
				} else if (type == "number") {
					json += value;
				} else if (type == "integer") {
					json += value;
				} else if (type == "boolean") {
					json += value;
				} else {
					// maybe more logs here or custom exception for debugging
					throw std::exception();
				}
				json += ",";
			} else if (req.HasHeader(param) && !req.GetHeader(param).empty()) {
				// maybe moving this if/else block to separate func
				json += "\"" + param + "\"" + ":";
				auto type = (*i)["type"].As<std::string>();
				auto value = req.GetHeader(param);
				if (type == "string") {
					json += "\"" + value + "\"";
				} else if (type == "number") {
					json += value;
				} else if (type == "integer") {
					json += value;
				} else if (type == "boolean") {
					json += value;
				} else {
					// maybe more logs here or custom exception for debugging
					throw std::exception();
				}
				json += ",";
			}
		}
		if (json.back() == ',')
			json.pop_back();
		json += "}";
		return json;
	}
}

void ValidateRequest(
	const server::http::HttpRequest& req,
	const std::map<server::http::HttpMethod, RequestAndJsonSchema>& map
) {
	auto schemas = map.at(req.GetMethod());
	std::string jsonFromReqStr = GenerateJson(schemas.request, req);

	try {
		formats::json::Value jsonFromReq = formats::json::FromString(jsonFromReqStr);
		formats::json::Schema schema(schemas.request);
		auto result = formats::json::Validate(jsonFromReq, schema);
		if (!result)
			throw errors::BadRequest400("Wrong params/headers");
	} catch (formats::json::ParseException& e) {
		throw errors::BadRequest400("Wrong params/headers");
	}
}

void ValidateBody(
	const std::map<server::http::HttpMethod, RequestAndJsonSchema>& map,
	server::http::HttpMethod method,
	const formats::json::Value& body
) {
	auto schemas = map.at(method);
	formats::json::Schema schema(schemas.json);
	auto result = formats::json::Validate(body, schema);
	if (!result)
		throw errors::BadRequest400("Wrong body");
}

} // svetit
