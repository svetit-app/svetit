#pragma once

#include <shared/errors_catchit.hpp>
#include <shared/parse/request.hpp>
#include <shared/schemas.hpp>
#include <shared/type_utils.hpp>

#include <map>

#include <type_traits>
#include <userver/formats/json/value.hpp>
#include <userver/components/component_config.hpp>
#include <userver/components/component_context.hpp>
#include <userver/server/handlers/http_handler_json_base.hpp>
#include <userver/utest/using_namespace_userver.hpp>
#include "userver/utils/boost_uuid4.hpp"

namespace svetit::project::handlers {

template<typename Service, typename T, const char* handlerName>
class SimpleCrud : public server::handlers::HttpHandlerJsonBase {
	using m = server::http::HttpMethod;
public:
	static constexpr std::string_view kName = handlerName;

	explicit SimpleCrud(
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
			// TODO: Add headers to API
			// - X-User
			// - X-Space-Id
			// - X-Space-Role
			const auto params = ValidateRequest(_mapHttpMethodToSchema, req, body);

			switch (req.GetMethod()) {
			case m::kGet:
				return Get(params);
			case m::kPost:
				return Post(req, body);
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
		auto table = _s.Repo().template Table<T>();
		using Table = typename std::remove_pointer_t<decltype(table)>;

		formats::json::ValueBuilder res;

		if (params.HasMember("key")) {
			if constexpr (HasGetByKey<Table>::value) {
				res = table->GetByKey(params["key"].As<std::string>());
				return res.ExtractValue();
			}
		}

		const auto id = getId(table, params);
		res = table->Get(id);
		return res.ExtractValue();
	}

	formats::json::Value Post(
		const server::http::HttpRequest& req,
		const formats::json::Value& body) const
	{
		formats::json::ValueBuilder res;
		const auto item = body.As<T>();

		auto table = _s.Repo().template Table<T>();
		res["id"] = table->Create(item);
		req.SetResponseStatus(server::http::HttpStatus::kCreated);
		return res.ExtractValue();
	}

	void Patch(
		const server::http::HttpRequest& req,
		const formats::json::Value& body) const
	{
		const auto item = body.As<T>();
		auto table = _s.Repo().template Table<T>();
		table->Update(item);

		req.SetResponseStatus(server::http::HttpStatus::kNoContent);
	}

	void Delete(const formats::json::Value& params) const
	{
		auto table = _s.Repo().template Table<T>();
		const auto id = getId(table, params);
		table->Delete(id);
	}

	template<typename Table>
	auto getId(Table*, const formats::json::Value& params)
	{
		using IdType = FuncArgT<decltype(&Table::Get)>;
		if constexpr (std::is_same<IdType, boost::uuids::uuid>::value) {
			return utils::BoostUuidFromString(params["id"].As<std::string>());
		} else {
			return params["id"].As<IdType>();
		}
	}

private:
	Service& _s;
	std::map<server::http::HttpMethod, RequestAndJsonSchema> _mapHttpMethodToSchema;
};

} // namespace svetit::project::handlers
