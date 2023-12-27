#pragma once

#include <string>
#include <string_view>

#include <userver/components/component_config.hpp>
#include <userver/components/component_context.hpp>
#include <userver/server/handlers/http_handler_json_base.hpp>
#include <userver/utest/using_namespace_userver.hpp>

namespace svetit::auth {
class Service;
} // namespace svetit::auth

namespace svetit::auth::handlers {

class UserList final : public server::handlers::HttpHandlerJsonBase {
public:
	static constexpr std::string_view kName = "handler-user-list";

	explicit UserList(
		const components::ComponentConfig& conf,
		const components::ComponentContext& ctx);

	formats::json::Value HandleRequestJsonThrow(
		const server::http::HttpRequest& req,
		const formats::json::Value& body,
		server::request::RequestContext&) const override;

private:
	Service& _s;
};

} // namespace svetit::auth::handlers
