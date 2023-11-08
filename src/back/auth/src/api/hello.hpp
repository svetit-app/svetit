#pragma once

#include <string>
#include <string_view>

#include <userver/server/handlers/http_handler_base.hpp>
#include <userver/utest/using_namespace_userver.hpp>

namespace svetit::auth::handlers {

std::string SayHelloTo(std::string_view name);

class Hello final : public server::handlers::HttpHandlerBase {
public:
	static constexpr std::string_view kName = "handler-hello";

	using HttpHandlerBase::HttpHandlerBase;

	std::string HandleRequestThrow(
		const server::http::HttpRequest &request,
		server::request::RequestContext &) const override;
};

} // namespace svetit::auth
