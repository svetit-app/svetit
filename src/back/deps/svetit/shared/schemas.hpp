#pragma once

#include <memory>
#include <string>
#include <map>

#include <userver/utest/using_namespace_userver.hpp>
#include <userver/server/handlers/http_handler_json_base.hpp>
#include <userver/formats/json/validate.hpp>

namespace svetit {

struct RequestAndJsonSchema {
	formats::json::Value requestProps;
	std::unique_ptr<formats::json::Schema> request, body;
};

std::map<server::http::HttpMethod, RequestAndJsonSchema> LoadSchemas(
	const std::string_view& handlerName, const std::string& schemasFolder);

void ValidateRequest(
	const std::map<server::http::HttpMethod, RequestAndJsonSchema>& schemasMap,
	const server::http::HttpRequest& req
);

void ValidateRequest(
	const std::map<server::http::HttpMethod, RequestAndJsonSchema>& schemasMap,
	const server::http::HttpRequest& req,
	const formats::json::Value& body
);

} // namespace svetit
