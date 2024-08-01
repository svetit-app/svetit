#pragma once

#include <map>

#include <shared/schemas.hpp>

#include <userver/components/component_config.hpp>
#include <userver/components/component_context.hpp>
#include <userver/server/handlers/http_handler_json_base.hpp>
#include <userver/utest/using_namespace_userver.hpp>

namespace svetit::node {
class Service;
} // namespace svetit::node

namespace svetit::node::handlers {

class GroupUser final : public server::handlers::HttpHandlerJsonBase {
public:
	static constexpr std::string_view kName = "handler-group-user";

	explicit GroupUser(
		const components::ComponentConfig& conf,
		const components::ComponentContext& ctx);

	formats::json::Value HandleRequestJsonThrow(
		const server::http::HttpRequest& req,
		const formats::json::Value& body,
		server::request::RequestContext&) const override;

	formats::json::Value Get(
		formats::json::ValueBuilder& res,
		const formats::json::Value& params) const;

	formats::json::Value Delete(
		const server::http::HttpRequest& req,
		formats::json::ValueBuilder& res,
		const formats::json::Value& params) const;

	formats::json::Value Post(
		const server::http::HttpRequest& req,
		const formats::json::Value& body,
		formats::json::ValueBuilder& res,
		const formats::json::Value& params) const;

private:
	Service& _s;
	std::map<server::http::HttpMethod, RequestAndJsonSchema> _mapHttpMethodToSchema;
};

} // namespace svetit::node::handlers