#pragma once

#include <string>
#include <map>

#include <userver/utest/using_namespace_userver.hpp>
#include <userver/server/handlers/http_handler_json_base.hpp>
#include <userver/formats/json/validate.hpp>

namespace svetit {

struct SchemasForMethod {
	std::string params, body;
};

void LoadSchemas(const std::string& handlerName, const std::string& schemasFolderPath, const server::http::HttpMethod& httpMethod, bool loadParams, bool loadBody, std::map<server::http::HttpMethod, SchemasForMethod>& _mapHttpMethodToSchema);

std::string GetBodySchemaFromRequestBody(
	const std::string& jsonSchemaParams,
	const std::string& path
);

std::string GenerateJsonDocument(
	const formats::json::Value& schemaDocumentParams,
	const server::http::HttpRequest& req
);

void ValidateRequest(const server::http::HttpRequest& req,  const std::map<server::http::HttpMethod, SchemasForMethod>& map);

} // namespace svetit