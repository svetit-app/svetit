#pragma once

#include "../errors.hpp"

#include <userver/utest/using_namespace_userver.hpp>

namespace svetit {

int parsePositiveInt(const server::http::HttpRequest& req, const std::string& key);

boost::uuids::uuid parseUUID(const server::http::HttpRequest& req, const std::string& key);

} // svetit