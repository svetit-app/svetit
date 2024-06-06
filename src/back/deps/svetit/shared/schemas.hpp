#pragma once

#include "userver/formats/json/value.hpp"
#include <memory>
#include <string>
#include <map>

#include <userver/utest/using_namespace_userver.hpp>
#include <userver/server/handlers/http_handler_json_base.hpp>

namespace svetit {

struct RequestAndJsonSchema {
	formats::json::Value requestProps;
	std::unique_ptr<formats::json::Schema> request, body;
};

std::map<server::http::HttpMethod, RequestAndJsonSchema> LoadSchemas(
	const std::string_view& handlerName, const std::string& schemasFolder);

formats::json::Value ValidateRequest(
	const std::map<server::http::HttpMethod, RequestAndJsonSchema>& schemasMap,
	const server::http::HttpRequest& req
);

formats::json::Value ValidateRequest(
	const std::map<server::http::HttpMethod, RequestAndJsonSchema>& schemasMap,
	const server::http::HttpRequest& req,
	const formats::json::Value& body
);

} // namespace svetit
