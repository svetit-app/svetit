#pragma once

#include <shared/errors_catchit.hpp>
#include <shared/parse/request.hpp>
#include <shared/parse/uuid.hpp>
#include <shared/schemas.hpp>
#include <shared/type_utils.hpp>
#include <shared/paging.hpp>
#include <shared/paging_serialize.hpp>
#include <shared/headers.hpp>

#include <map>

#include <userver/components/component_config.hpp>
#include <userver/components/component_context.hpp>
#include <userver/server/handlers/http_handler_json_base.hpp>
#include <userver/utest/using_namespace_userver.hpp>

namespace svetit::project::handlers {

template<typename Service, typename T, const char* handlerName, const char* filterKey = nullptr>
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
			// - X-Space-Role
			const auto params = ValidateRequest(_mapHttpMethodToSchema, req, body);

			auto table = _s.Repo().template Table<T>();
			return getList(table, params);
		} catch(...) {
			return errors::CatchIt(req);
		}

		return {};
	}

	template<typename Table>
	auto getList(Table* table, const formats::json::Value& params) const
	{
		using FuncInfo = FunctionTraits<decltype(&Table::GetList)>;

		const auto paging = parsePaging(params);

		typename FuncInfo::tuple args;
		std::get<0>(args) = params[headers::kSpaceId].As<boost::uuids::uuid>();
		std::get<FuncInfo::nargs - 2>(args) = paging.start;
		std::get<FuncInfo::nargs - 1>(args) = paging.limit;

		if constexpr (filterKey != nullptr) {
			using FilterType = typename FuncInfo::template arg<1>::type;
			std::get<1>(args) = params[filterKey].As<FilterType>();
		}

		formats::json::ValueBuilder res;
		res = std::apply(std::bind_front(&Table::GetList, table), args);
		return res.ExtractValue();
	}

private:
	Service& _s;
	std::map<server::http::HttpMethod, RequestAndJsonSchema> _mapHttpMethodToSchema;
};

} // namespace svetit::project::handlers
