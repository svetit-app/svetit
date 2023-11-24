#pragma once

#include "errors.hpp"

#include <userver/components/component_config.hpp>
#include <userver/components/component_context.hpp>
#include <userver/server/handlers/http_handler_json_base.hpp>
#include <userver/utest/using_namespace_userver.hpp>

namespace svetit {

struct Paging {
	int start, limit;
};

template<typename T>
struct PagingResult {
	std::vector<T> items;
	int64_t total = 0;
};

static Paging parsePaging(const server::http::HttpRequest& req);

static int parsePositiveInt(const server::http::HttpRequest& req, const std::string& key);

} // svetit