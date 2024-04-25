#pragma once

#include <string>
#include <map>

#include <userver/utest/using_namespace_userver.hpp>
#include <userver/server/handlers/http_handler_json_base.hpp>
#include <userver/formats/json/validate.hpp>

namespace svetit {

struct RequestAndJsonSchema {
	formats::json::Value request, json;
};

void LoadSchemas(
	const std::string& handlerName,
	const std::string& schemasFolder,
	const server::http::HttpMethod& method,
	bool loadParams,
	bool loadBody,
	std::map<server::http::HttpMethod, RequestAndJsonSchema>& map);

formats::json::Value GetBodySchema(
	const std::string& requestSchemaStr,
	const std::string& path
);

std::string GenerateJson(
	const formats::json::Value& requestSchema,
	const server::http::HttpRequest& req
);

void ValidateRequest(
	const server::http::HttpRequest& req,
	const std::map<server::http::HttpMethod, RequestAndJsonSchema>& map
);

void ValidateBody(
	const std::map<server::http::HttpMethod, RequestAndJsonSchema>& map,
	server::http::HttpMethod method,
	const formats::json::Value& body
);

} // namespace svetit