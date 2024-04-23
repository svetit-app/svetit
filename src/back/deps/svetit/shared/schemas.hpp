#pragma once

#include <string>

#include <userver/utest/using_namespace_userver.hpp>
#include <userver/server/handlers/http_handler_json_base.hpp>
#include <userver/formats/json/validate.hpp>

namespace svetit {

struct SchemasForMethod {
	std::string params, body;
};

std::string GenerateJsonDocument(
	const formats::json::Value& schemaDocumentParams,
	const server::http::HttpRequest& req
);

std::string GetBodySchemaFromRequestBody(
	const std::string& jsonSchemaParams,
	const std::string& path
);

} // namespace svetit