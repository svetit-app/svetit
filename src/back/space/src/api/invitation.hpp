#pragma once

#include <string>
#include <boost/lexical_cast.hpp>
#include "../model/invitation_serialize.hpp"

#include <userver/components/component_config.hpp>
#include <userver/components/component_context.hpp>
#include <userver/server/handlers/http_handler_json_base.hpp>
#include <userver/utest/using_namespace_userver.hpp>

namespace svetit::space {
class Service;
} // namespace svetit::space

namespace svetit::space::handlers {

class Invitation final : public server::handlers::HttpHandlerJsonBase {
public:
	static constexpr std::string_view kName = "handler-invitation";

	explicit Invitation(
		const components::ComponentConfig& conf,
		const components::ComponentContext& ctx);

	formats::json::Value HandleRequestJsonThrow(
		const server::http::HttpRequest& req,
		const formats::json::Value& body,
		server::request::RequestContext&) const;

	formats::json::Value GetList(
	const server::http::HttpRequest& req,
	const formats::json::Value& body) const;

	formats::json::Value Post(
	const server::http::HttpRequest& req,
	const formats::json::Value& body) const;

	formats::json::Value ChangeRole(
		const server::http::HttpRequest& req,
		const formats::json::Value& body) const;

	formats::json::Value Join(
		const server::http::HttpRequest& req,
		const formats::json::Value& body) const;

	formats::json::Value Delete(
		const server::http::HttpRequest& req,
		const formats::json::Value& body) const;

private:
	Service& _s;
};

} // namespace svetit::space::handlers