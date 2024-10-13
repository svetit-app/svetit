#pragma once

#include <userver/server/handlers/http_handler_json_base.hpp>
#include <userver/utest/using_namespace_userver.hpp>

namespace svetit::errors {

bool IsStatusForLogging(server::http::HttpStatus status);

struct Exception : public std::runtime_error {
	Exception(const std::string& text, server::http::HttpStatus status);
	server::http::HttpStatus GetHttpStatus() const;
	bool NeedsLogging() const;
private:
	server::http::HttpStatus _status;
};

struct NotFound404 : public Exception {
	NotFound404();
};
struct Unauthorized401 : public Exception {
	Unauthorized401();
};
struct Conflict409 : public Exception {
	Conflict409(const std::string& text);
};
struct BadRequest400 : public Exception {
	BadRequest400(const std::string& text);
};
struct NotModified304 : public Exception {
	NotModified304();
};
struct Forbidden403 : public Exception {
	Forbidden403();
};

} // namespace svetit::errors
