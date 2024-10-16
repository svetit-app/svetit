#pragma once

#include <map>

#include <svetit/schemas.hpp>

#include <userver/components/component_config.hpp>
#include <userver/components/component_context.hpp>
#include <userver/server/handlers/http_handler_json_base.hpp>
#include <userver/utest/using_namespace_userver.hpp>

namespace svetit::node {
class Service;
} // namespace svetit::node

namespace svetit::node::handlers {

class NodeGroupList final : public server::handlers::HttpHandlerJsonBase {
public:
	static constexpr std::string_view kName = "handler-node-group-list";

	explicit NodeGroupList(
		const components::ComponentConfig& conf,
		const components::ComponentContext& ctx);

	formats::json::Value HandleRequestJsonThrow(
		const server::http::HttpRequest& req,
		const formats::json::Value& body,
		server::request::RequestContext&) const override;

private:
	Service& _s;
	std::map<server::http::HttpMethod, RequestAndJsonSchema> _mapHttpMethodToSchema;
};

} // namespace svetit::node::handlers