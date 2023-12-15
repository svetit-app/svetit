#include "errors.hpp"

namespace svetit::errors {

// Exception - базовый класс исключений
Exception::Exception(const std::string& text, server::http::HttpStatus status)
	: std::runtime_error(text)
	, _status{status} {}

server::http::HttpStatus Exception::GetHttpStatus() const { return _status; }

// NotFound404
NotFound404::NotFound404()
	: Exception{"resource not found", server::http::HttpStatus::kNotFound} {}

// Unauthorized401
Unauthorized401::Unauthorized401()
	: Exception{"bad authorization token", server::http::HttpStatus::kUnauthorized} {}

// Conflict409
Conflict409::Conflict409(const std::string& text)
	: Exception{text, server::http::HttpStatus::kConflict} {}

// BadRequest400
BadRequest400::BadRequest400(const std::string& text)
	: Exception{text, server::http::HttpStatus::kBadRequest} {}

// NotModified304
NotModified304::NotModified304()
	: Exception{"Not modified", server::http::HttpStatus::kNotModified} {}

// Forbidden403
Forbidden403::Forbidden403()
	: Exception{"Access forbidden", server::http::HttpStatus::kForbidden} {}

} // namespace svetit::errors