#pragma once

#include <userver/components/component_config.hpp>
#include <userver/components/component_context.hpp>
#include <userver/server/handlers/http_handler_json_base.hpp>
#include <userver/utest/using_namespace_userver.hpp>
#include <userver/utils/boost_uuid4.hpp>

namespace svetit::space {
class Service;
} // namespace svetit::space

namespace svetit::space::handlers {

class UserManage final : public server::handlers::HttpHandlerJsonBase {
public:
	static constexpr std::string_view kName = "handler-user-manage";

	explicit UserManage(
		const components::ComponentConfig& conf,
		const components::ComponentContext& ctx);

	formats::json::Value HandleRequestJsonThrow(
		const server::http::HttpRequest& req,
		const formats::json::Value& body,
		server::request::RequestContext&) const override;

	formats::json::Value Delete(
		const std::string headerUserId,
		const server::http::HttpRequest& req,
		const formats::json::Value& body) const;

	formats::json::Value UpdateUser(
		const std::string headerUserId,
		const server::http::HttpRequest& req,
		const formats::json::Value& body) const;

private:
	Service& _s;
};

} // namespace svetit::space::handlers