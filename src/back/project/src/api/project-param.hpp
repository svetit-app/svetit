#pragma once

#include <string>

#include <userver/components/component_config.hpp>
#include <userver/components/component_context.hpp>
#include <userver/server/handlers/http_handler_json_base.hpp>
#include <userver/utest/using_namespace_userver.hpp>

namespace svetit::project {
class Service;
} // namespace svetit::project

namespace svetit::project::handlers {

class ProjectParam final : public server::handlers::HttpHandlerJsonBase {
public:
	static constexpr std::string_view kName = "handler-project-param";

	explicit ProjectParam(
		const components::ComponentConfig& conf,
		const components::ComponentContext& ctx);

	formats::json::Value HandleRequestJsonThrow(
		const server::http::HttpRequest& req,
		const formats::json::Value& body,
		server::request::RequestContext&) const override;

	formats::json::Value Get(
		const server::http::HttpRequest& req,
		formats::json::ValueBuilder& res) const;

	formats::json::Value Post(
		const server::http::HttpRequest& req,
		const formats::json::Value& body,
		formats::json::ValueBuilder& res) const;

	formats::json::Value Delete(
		const server::http::HttpRequest& req,
		formats::json::ValueBuilder& res) const;

private:
	Service& _s;
};

} // namespace svetit::project::handlers