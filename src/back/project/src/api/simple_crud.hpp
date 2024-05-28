#pragma once

#include <boost/uuid/uuid.hpp>
#include <shared/errors_catchit.hpp>
#include <shared/parse/request.hpp>
#include <shared/parse/uuid.hpp>
#include <shared/schemas.hpp>
#include <shared/type_utils.hpp>
#include <shared/headers.hpp>

#include <map>

#include <type_traits>
#include <userver/formats/json/value.hpp>
#include <userver/components/component_config.hpp>
#include <userver/components/component_context.hpp>
#include <userver/server/handlers/http_handler_json_base.hpp>
#include <userver/utest/using_namespace_userver.hpp>

namespace svetit::project::handlers {

static constexpr std::string_view kIdKey = "id";

template<typename Service, typename T, const std::string_view* handlerName, const std::string_view* idKey = kIdKey>
class SimpleCrud : public server::handlers::HttpHandlerJsonBase {
	using RepoT = std::remove_reference_t<ReturnTypeT<decltype(&Service::Repo)>>;
	using Table = std::remove_pointer_t<ReturnTypeT<decltype(&RepoT::template Table<T>)>>;
	using GetFuncInfo = FunctionTraits<decltype(&Table::Get)>;
	using IdType = typename GetFuncInfo::template arg<1>::type;
public:
	static constexpr std::string_view kName = *handlerName;

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
			const auto params = ValidateRequest(_mapHttpMethodToSchema, req, body);
			const auto spaceId = params[headers::kSpaceId].As<boost::uuids::uuid>();
			auto table = _s.Repo().template Table<T>();

			using m = server::http::HttpMethod;
			switch (req.GetMethod()) {
			case m::kGet:
				return Get(table, params, spaceId);
			case m::kPost:
				return Post(table, req, spaceId, body);
			case m::kPatch:
				Patch(table, req, spaceId, body);
				break;
			case m::kDelete:
				Delete(table, params, spaceId);
				break;
			default:
				throw std::runtime_error("Unsupported");
			}
		} catch(...) {
			return errors::CatchIt(req);
		}

		return {};
	}

	formats::json::Value Get(Table* table, const formats::json::Value& params, const boost::uuids::uuid& spaceId) const
	{
		formats::json::ValueBuilder res;

		if (params.HasMember("key")) {
			if constexpr (HasGetByKey<Table>::value) {
				res = table->GetByKey(spaceId, params["key"].As<std::string>());
				return res.ExtractValue();
			}
		}

		typename GetFuncInfo::tuple args;
		std::get<0>(args) = spaceId;
		std::get<1>(args) = params[*idKey].As<IdType>();

		// Если у функции Get 3 аргумента, считаем что у таблицы составной ключ
		if constexpr (GetFuncInfo::nargs == 3) {
			using Id2Type = typename GetFuncInfo::template arg<2>::type;
			std::get<2>(args) = getId2<Id2Type>(params);
		}

		res = std::apply(std::bind_front(&Table::Get, table), args);
		return res.ExtractValue();
	}

	formats::json::Value Post(
		Table* table,
		const server::http::HttpRequest& req,
		const boost::uuids::uuid& spaceId,
		const formats::json::Value& body) const
	{
		formats::json::ValueBuilder res;
		auto item = body.As<T>();
		item.spaceId = spaceId;

		if constexpr(std::is_same_v<void, ReturnTypeT<decltype(&Table::Create)>>) {
			table->Create(item);
		} else {
			res[std::string{*idKey}] = table->Create(item);
		}

		req.SetResponseStatus(server::http::HttpStatus::kCreated);
		return res.ExtractValue();
	}

	void Patch(
		Table* table,
		const server::http::HttpRequest& req,
		const boost::uuids::uuid& spaceId,
		const formats::json::Value& body) const
	{
		auto item = body.As<T>();
		item.spaceId = spaceId;

		table->Update(item);
		req.SetResponseStatus(server::http::HttpStatus::kNoContent);
	}

	void Delete(Table* table, const formats::json::Value& params, const boost::uuids::uuid& spaceId) const
	{
		using FuncInfo = FunctionTraits<decltype(&Table::Delete)>;

		typename FuncInfo::tuple args;
		std::get<0>(args) = spaceId;
		std::get<1>(args) = params[*idKey].As<IdType>();

		// Если у функции Delete 3 аргумента, считаем что у таблицы составной ключ
		if constexpr (FuncInfo::nargs == 3) {
			using Id2Type = typename FuncInfo::template arg<2>::type;
			std::get<2>(args) = getId2<Id2Type>(params);
		}

		std::apply(std::bind_front(&Table::Delete, table), args);
	}

	template<typename Id2Type>
	Id2Type getId2(const formats::json::Value& params) const
	{
		// idKey - это имя первого Id в таблице
		// перебираем все параметры, ищем первый который не X-* и не idKey
		for (auto it = params.begin(); it != params.end(); ++it)
		{
			const auto name = it.GetName();
			if (!name.empty() && name.rfind("X-", 0) != 0 && name != *idKey)
				return it->As<Id2Type>();
		}

		return Id2Type{};
	}

protected:
	Service& _s;
	std::map<server::http::HttpMethod, RequestAndJsonSchema> _mapHttpMethodToSchema;
};

} // namespace svetit::project::handlers
