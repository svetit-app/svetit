#pragma once

#include "errors_catchit.hpp"
#include "parse/request.hpp"
#include "schemas.hpp"
#include "userver/formats/json/value.hpp"

#include <map>

#include <userver/components/component_config.hpp>
#include <userver/components/component_context.hpp>
#include <userver/server/handlers/http_handler_json_base.hpp>
#include <userver/utest/using_namespace_userver.hpp>

namespace svetit {

template<typename Service, typename T, const char* handlerName>
class SimpleApiHandler : public server::handlers::HttpHandlerJsonBase {
	typedef int64_t IdType;
	using m = server::http::HttpMethod;
public:
	static constexpr std::string_view kName = handlerName;

	explicit SimpleApiHandler(
		const components::ComponentConfig& conf,
		const components::ComponentContext& ctx)
		: server::handlers::HttpHandlerJsonBase{conf, ctx}
		, _s{ctx.FindComponent<Service>()}
		, _mapHttpMethodToSchema{LoadSchemas(kName, _s.GetJSONSchemasPath())} {}

	formats::json::Value HandleRequestJsonThrow(
		const server::http::HttpRequest& req,
		const formats::json::Value& body,
		server::request::RequestContext&) const override
	{
		try {
			const auto params = ValidateRequest(_mapHttpMethodToSchema, req, body);

			switch (req.GetMethod()) {
			case m::kGet:
				return Get(params);
			case m::kPost:
				return Post(body);
			case m::kPatch:
				Patch(req, body);
				break;
			case m::kDelete:
				Delete(params);
				break;
			default:
				throw std::runtime_error("Unsupported");
			}
		} catch(...) {
			return errors::CatchIt(req);
		}

		return {};
	}

	formats::json::Value Get(const formats::json::Value& params) const
	{
		formats::json::ValueBuilder res;
		const auto id = params["id"].As<IdType>();
		res = _s.template Process<m::kGet, T>(id);
		return res.ExtractValue();
	}

	formats::json::Value Post(const formats::json::Value& body) const
	{
		formats::json::ValueBuilder res;
		const auto item = body.As<T>();
		res["id"] = _s.template Process<m::kPost, T, IdType>(item);
		return res.ExtractValue();
	}

	void Patch(
		const server::http::HttpRequest& req,
		const formats::json::Value& body) const
	{
		const auto item = body.As<T>();
		_s.template Process<m::kPatch, T, void>(item);

		req.SetResponseStatus(server::http::HttpStatus::kNoContent);
	}

	void Delete(const formats::json::Value& params) const
	{
		const auto id = params["id"].As<IdType>();
		_s.template Process<m::kDelete, T, void>(id);
	}

private:
	Service& _s;
	std::map<server::http::HttpMethod, RequestAndJsonSchema> _mapHttpMethodToSchema;
};

} // namespace svetit::project::handlers
