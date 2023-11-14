#pragma once

#include "errors.hpp"

#include <userver/components/component_config.hpp>
#include <userver/components/component_context.hpp>
#include <userver/server/handlers/http_handler_json_base.hpp>
#include <userver/utest/using_namespace_userver.hpp>

namespace svetit::paging {

struct Paging {
	int start, limit;
};

static Paging parsePaging(const server::http::HttpRequest& req);

} // svetit::paging