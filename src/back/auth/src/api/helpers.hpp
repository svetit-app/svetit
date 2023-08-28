#pragma once

#include <userver/server/http/http_request.hpp>
#include <userver/utest/using_namespace_userver.hpp>

namespace svetit::auth::handlers {

std::string getCallerUrl(const server::http::HttpRequest& req, bool addApiPrefix = false);

std::string getCallerUserAgent(const server::http::HttpRequest& req);

} // namespace svetit::auth::handlers
