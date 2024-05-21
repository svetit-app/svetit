#pragma once

#include <shared/errors_catchit.hpp>
#include <shared/parse/request.hpp>
#include <shared/schemas.hpp>
#include <shared/type_utils.hpp>
#include <shared/paging.hpp>
#include <shared/paging_serialize.hpp>

#include <map>

#include <type_traits>
#include <userver/formats/json/value.hpp>
#include <userver/components/component_config.hpp>
#include <userver/components/component_context.hpp>
#include <userver/server/handlers/http_handler_json_base.hpp>
#include <userver/utest/using_namespace_userver.hpp>
#include <userver/utils/boost_uuid4.hpp>

namespace svetit::project::handlers {

template<typename Service, typename T, const char* handlerName, const char* filterKey>
class SimpleList : public server::handlers::HttpHandlerJsonBase {
	using m = server::http::HttpMethod;
public:
	static constexpr std::string_view kName = handlerName;

	explicit SimpleList(
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
			const auto paging = parsePaging(req);
			const auto spaceId = utils::BoostUuidFromString(params["X-Space-Id"].As<std::string>());

			auto table = _s.Repo().template Table<T>();
			const auto filterValue = getFilterValue(table, params);

			formats::json::ValueBuilder res;
			res = table->GetList(spaceId, filterValue, paging.start, paging.limit);
			return res.ExtractValue();
		} catch(...) {
			return errors::CatchIt(req);
		}

		return {};
	}

	template<typename Table>
	auto getFilterValue(Table*, const formats::json::Value& params) const
	{
		const auto param = params[filterKey];

		using IdType = FuncArgT<decltype(&Table::GetList), 3>;
		if constexpr (std::is_same<IdType, boost::uuids::uuid>::value) {
			return utils::BoostUuidFromString(param.As<std::string>());
		} else {
			return param.As<IdType>();
		}
	}

private:
	Service& _s;
	std::map<server::http::HttpMethod, RequestAndJsonSchema> _mapHttpMethodToSchema;
};

} // namespace svetit::project::handlers
