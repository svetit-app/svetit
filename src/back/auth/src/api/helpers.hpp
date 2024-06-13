#pragma once

#include <userver/server/http/http_request.hpp>
#include <userver/utest/using_namespace_userver.hpp>
#include <userver/formats/json/value.hpp>

namespace svetit::auth::handlers {

std::string getCallerUrl(
	const server::http::HttpRequest& req,
	const formats::json::Value& params,
	bool addApiPrefix = false);

} // namespace svetit::auth::handlers
