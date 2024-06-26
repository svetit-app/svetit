#pragma once

#include <shared/errors_catchit.hpp>
#include <shared/parse/request.hpp>
#include <shared/parse/uuid.hpp>
#include <shared/schemas.hpp>
#include <shared/type_utils.hpp>
#include <shared/paging.hpp>
#include <shared/paging_serialize.hpp>
#include <shared/headers.hpp>
#include <shared/utils/handlername.hpp>
#include <shared/db/utils/gettupleofids.hpp>
#include <shared/db/utils/idstuple.hpp>

#include <userver/components/component_config.hpp>
#include <userver/components/component_context.hpp>
#include <userver/server/handlers/http_handler_json_base.hpp>
#include <userver/utest/using_namespace_userver.hpp>

#include <boost/pfr/core_name.hpp>

namespace svetit::handlers {

template<typename Service, typename T>
class SimpleList2 : public server::handlers::HttpHandlerJsonBase {
	static constexpr auto kNameArr = utils::HandlerName<T>("handler-", "-list", '-');
public:
	static constexpr std::string_view kName = kNameArr.data();

	explicit SimpleList2(
		const components::ComponentConfig& conf,
		const components::ComponentContext& ctx);

	formats::json::Value HandleRequestJsonThrow(
		const server::http::HttpRequest& req,
		const formats::json::Value& body,
		server::request::RequestContext&) const override;

	template<typename Table>
	auto getList(Table* table, const formats::json::Value& params) const;

protected:
	Service& _s;
	std::map<server::http::HttpMethod, RequestAndJsonSchema> _mapHttpMethodToSchema;
};

template<typename Service, typename T>
inline SimpleList2<Service, T>::SimpleList2(
	const components::ComponentConfig& conf,
	const components::ComponentContext& ctx)
	: server::handlers::HttpHandlerJsonBase{conf, ctx}
	, _s{ctx.FindComponent<Service>()}
	, _mapHttpMethodToSchema{LoadSchemas(kName, _s.GetJSONSchemasPath())} {}

template<typename Service, typename T>
inline formats::json::Value SimpleList2<Service, T>::HandleRequestJsonThrow(
	const server::http::HttpRequest& req,
	const formats::json::Value& body,
	server::request::RequestContext&) const
{
	try {
		const auto params = ValidateRequest(_mapHttpMethodToSchema, req, body);

		auto table = _s.Repo().template Table<T>();
		return getList(table, params);
	} catch(...) {
		return errors::CatchIt(req);
	}

	return {};
}

template<typename Service, typename T>
template<typename Table>
inline auto SimpleList2<Service, T>::getList(Table* table, const formats::json::Value& params) const
{
	LOG_ERROR() << "BEGIN";
	const auto paging = parsePaging(params);
	const auto pagingArgs = std::make_tuple(paging.start, paging.limit);

	LOG_ERROR() << "BEGIN 2";
	const auto names = boost::pfr::names_as_array<T>();
	const auto idsIndexes = db::utils::FilterIdsTuple<T>::Get();
	auto tailArgs = db::utils::GetTupleOfIds<T, typename db::utils::FilterIdsTuple<T>::type>(names, params, idsIndexes);
	LOG_ERROR() << "BEGIN 3";

	auto args = std::tuple_cat(pagingArgs, tailArgs);

	LOG_ERROR() << "BEGIN 4";
	formats::json::ValueBuilder res;
	res = table->CallGetList(args);
	LOG_ERROR() << "BEGIN 5";
	return res.ExtractValue();
}

} // namespace svetit::handlers
