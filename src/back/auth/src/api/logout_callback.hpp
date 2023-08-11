#pragma once

#include <string>
#include <string_view>

#include <userver/components/component_config.hpp>
#include <userver/components/component_context.hpp>
#include <userver/server/handlers/http_handler_base.hpp>
#include <userver/utest/using_namespace_userver.hpp>

namespace svetit::auth {
class Service;
} // namespace svetit::auth

namespace svetit::auth::handlers {

class LogoutCallback final : public server::handlers::HttpHandlerBase {
public:
	static constexpr std::string_view kName = "handler-logout-callback";

	LogoutCallback(
		const components::ComponentConfig& conf,
		const components::ComponentContext& ctx);

	std::string HandleRequestThrow(
		const server::http::HttpRequest& req,
		server::request::RequestContext&) const override;

private:
	Service& _s;
};

} // namespace svetit::auth::handlers
