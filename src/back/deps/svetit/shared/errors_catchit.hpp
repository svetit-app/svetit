#pragma once

#include "errors.hpp"

#include <userver/server/handlers/http_handler_json_base.hpp>
#include <userver/utest/using_namespace_userver.hpp>

namespace svetit::errors {

formats::json::Value CatchIt(const server::http::HttpRequest& req);

} // namespace svetit::errors
