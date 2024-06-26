#pragma once

#include "userver/formats/json/value_builder.hpp"
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
	using RepoT = std::remove_reference_t<ReturnTypeT<decltype(&Service::Repo)>>;
	using Table = std::remove_pointer_t<ReturnTypeT<decltype(&RepoT::template Table<T>)>>;
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

	formats::json::Value getList(formats::json::Value&& params) const;

protected:
	Table* _table;
	std::map<server::http::HttpMethod, RequestAndJsonSchema> _mapHttpMethodToSchema;
};

template<typename Service, typename T>
inline SimpleList2<Service, T>::SimpleList2(
	const components::ComponentConfig& conf,
	const components::ComponentContext& ctx)
	: server::handlers::HttpHandlerJsonBase{conf, ctx}
	, _table{ctx.FindComponent<Service>().Repo().template Table<T>()}
	, _mapHttpMethodToSchema{LoadSchemas(kName, ctx.FindComponent<Service>().GetJSONSchemasPath())} {}

template<typename Service, typename T>
inline formats::json::Value SimpleList2<Service, T>::HandleRequestJsonThrow(
	const server::http::HttpRequest& req,
	const formats::json::Value& body,
	server::request::RequestContext&) const
{
	try {
		auto params = ValidateRequest(_mapHttpMethodToSchema, req, body);
		return getList(std::move(params));
	} catch(...) {
		return errors::CatchIt(req);
	}

	return {};
}

template<typename Service, typename T>
inline formats::json::Value SimpleList2<Service, T>::getList(formats::json::Value&& params) const
{
	formats::json::ValueBuilder paramsBuilder(std::move(params));
	if (paramsBuilder.HasMember(headers::kSpaceId) && !paramsBuilder.HasMember("spaceId"))
		paramsBuilder["spaceId"] = paramsBuilder[std::string(headers::kSpaceId)];
	params = paramsBuilder.ExtractValue();

	const auto paging = parsePaging(params);
	const auto pagingArgs = std::make_tuple(paging.start, paging.limit);

	const auto names = boost::pfr::names_as_array<T>();
	const auto idsIndexes = db::utils::FilterIdsTuple<T>::Get();
	auto tailArgs = db::utils::GetTupleOfIds<T, typename db::utils::FilterIdsTuple<T>::type>(names, params, idsIndexes);

	auto args = std::tuple_cat(pagingArgs, tailArgs);

	formats::json::ValueBuilder res;
	res = _table->CallGetList(args);
	return res.ExtractValue();
}

} // namespace svetit::handlers
