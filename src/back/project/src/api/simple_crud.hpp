#pragma once

#include "../service/service.hpp"

#include <shared/errors_catchit.hpp>
#include <shared/parse/uuid.hpp>
#include <shared/schemas.hpp>
#include <shared/type_utils.hpp>
#include <shared/headers.hpp>
#include <shared/mapheadertoparams.hpp>
#include <shared/utils/handlername.hpp>
#include <shared/db/utils/gettupleofids.hpp>
#include <shared/db/utils/idstuple.hpp>

#include <userver/components/component_config.hpp>
#include <userver/components/component_context.hpp>
#include <userver/server/handlers/http_handler_json_base.hpp>
#include <userver/utest/using_namespace_userver.hpp>

#include <boost/uuid/uuid.hpp>
#include <boost/pfr/core_name.hpp>

namespace svetit::project::handlers {

template<typename T>
class SimpleCrud : public server::handlers::HttpHandlerJsonBase {
	using RepoT = std::remove_reference_t<ReturnTypeT<decltype(&Service::Repo)>>;
	using Table = std::remove_pointer_t<ReturnTypeT<decltype(&RepoT::template Table<T>)>>;
	static constexpr auto kNameArr = utils::HandlerName<T>("handler-", "", '-');
public:
	static constexpr std::string_view kName = kNameArr.data();

	explicit SimpleCrud(
		const components::ComponentConfig& conf,
		const components::ComponentContext& ctx);

	formats::json::Value HandleRequestJsonThrow(
		const server::http::HttpRequest& req,
		const formats::json::Value& body,
		server::request::RequestContext&) const override;

	formats::json::Value Get(
		formats::json::Value&& params) const;

	formats::json::Value Post(
		const server::http::HttpRequest& req,
		const formats::json::Value& params,
		const formats::json::Value& body) const;

	void Patch(
		const server::http::HttpRequest& req,
		const formats::json::Value& params,
		const formats::json::Value& body) const;

	void Delete(
		const server::http::HttpRequest& req,
		formats::json::Value&& params) const;

protected:
	Table* _table;
	std::map<server::http::HttpMethod, RequestAndJsonSchema> _mapHttpMethodToSchema;
};

template<typename T>
inline SimpleCrud<T>::SimpleCrud(
	const components::ComponentConfig& conf,
	const components::ComponentContext& ctx)
	: server::handlers::HttpHandlerJsonBase{conf, ctx}
	, _table{ctx.FindComponent<Service>().Repo().template Table<T>()}
	, _mapHttpMethodToSchema{LoadSchemas(kName, ctx.FindComponent<Service>().GetJSONSchemasPath())} {}

template<typename T>
inline formats::json::Value SimpleCrud<T>::HandleRequestJsonThrow(
	const server::http::HttpRequest& req,
	const formats::json::Value& body,
	server::request::RequestContext&) const
{
	try {
		auto params = ValidateRequest(_mapHttpMethodToSchema, req, body);

		using m = server::http::HttpMethod;
		switch (req.GetMethod()) {
		case m::kGet:
			return Get(std::move(params));
		case m::kPost:
			return Post(req, params, body);
		case m::kPatch:
			Patch(req, params, body);
			break;
		case m::kDelete:
			Delete(req, std::move(params));
			break;
		default:
			throw std::runtime_error("Unsupported");
		}
	} catch(...) {
		return errors::CatchIt(req);
	}

	return {};
}

template<typename T>
inline formats::json::Value SimpleCrud<T>::Get(formats::json::Value&& params) const
{
	formats::json::ValueBuilder res;
	if (params.HasMember("key")) {
		if constexpr (HasGetByKey<Table>::value) {
			const auto spaceId = params[headers::kSpaceId].As<boost::uuids::uuid>();
			res = _table->GetByKey(spaceId, params["key"].As<std::string>());
			return res.ExtractValue();
		}
	}

	MapHeaderToParams(params);

	const auto names = boost::pfr::names_as_array<T>();
	const auto idsIndexes = db::utils::IdsTuple<T>::Get();
	const auto args = db::utils::GetTupleOfIds<T, typename db::utils::IdsTuple<T>::type>(names, params, idsIndexes);

	res = _table->CallGet(args);
	return res.ExtractValue();
}

template<typename T>
inline formats::json::Value SimpleCrud<T>::Post(
	const server::http::HttpRequest& req,
	const formats::json::Value& params,
	const formats::json::Value& body) const
{
	formats::json::ValueBuilder res;
	auto item = body.As<T>();

	if constexpr (HasSpaceIdField<T>::value)
		item.spaceId = params[headers::kSpaceId].As<boost::uuids::uuid>();

	constexpr auto names = boost::pfr::names_as_array<T>();
	res[names.front().data()] = _table->Create(item);

	req.SetResponseStatus(server::http::HttpStatus::kCreated);
	return res.ExtractValue();
}

template<typename T>
inline void SimpleCrud<T>::Patch(
	const server::http::HttpRequest& req,
	const formats::json::Value& params,
	const formats::json::Value& body) const
{
	auto item = body.As<T>();
	if constexpr (HasSpaceIdField<T>::value)
		item.spaceId = params[headers::kSpaceId].As<boost::uuids::uuid>();

	_table->Update(item);
	req.SetResponseStatus(server::http::HttpStatus::kNoContent);
}

template<typename T>
inline void SimpleCrud<T>::Delete(
	const server::http::HttpRequest& req,
	formats::json::Value&& params) const
{
	MapHeaderToParams(params);

	const auto names = boost::pfr::names_as_array<T>();
	const auto idsIndexes = db::utils::IdsTuple<T>::Get();
	const auto args = db::utils::GetTupleOfIds<T, typename db::utils::IdsTuple<T>::type>(names, params, idsIndexes);

	_table->CallDelete(args);
	req.SetResponseStatus(server::http::HttpStatus::kNoContent);
}

} // namespace svetit::project::handlers
