#include "role.hpp"
#include "../model/role_serialize.hpp"
#include "../service/service.hpp"
#include "../repo/repository.hpp"
#include <svetit/headers.hpp>
#include <svetit/errors.hpp>
#include <svetit/errors_catchit.hpp>
#include <svetit/paging.hpp>
#include <svetit/parse/request.hpp>
#include <svetit/schemas.hpp>
#include <svetit/parse/uuid.hpp>

#include <boost/lexical_cast.hpp>

namespace svetit::space::handlers {

Role::Role(
	const components::ComponentConfig& conf,
	const components::ComponentContext& ctx)
	: server::handlers::HttpHandlerJsonBase{conf, ctx}
	, _s{ctx.FindComponent<Service>()}
	, _mapHttpMethodToSchema{LoadSchemas(kName, _s.GetJSONSchemasPath())}
{}

formats::json::Value Role::HandleRequestJsonThrow(
	const server::http::HttpRequest& req,
	const formats::json::Value& body,
	server::request::RequestContext&) const
{
	formats::json::ValueBuilder res;

	try {
		const auto params = ValidateRequest(_mapHttpMethodToSchema, req, body);

		switch (req.GetMethod()) {
			case server::http::HttpMethod::kGet:
				return Get(res, params);
			case server::http::HttpMethod::kPost:
				return Post(req, body, res, params);
			case server::http::HttpMethod::kPut:
				return Put(req, body, res, params);
			case server::http::HttpMethod::kDelete:
				return Delete(req, res, params);
			default:
				throw std::runtime_error("Unsupported");
				break;
		}
	} catch(...) {
		return errors::CatchIt(req);
	}

	return res.ExtractValue();
}

formats::json::Value Role::Get(
	formats::json::ValueBuilder& res,
	const formats::json::Value& params) const
{
	const auto spaceId = params[headers::kSpaceId].As<boost::uuids::uuid>();
	const auto id = params["id"].As<int>();
	res = _s.Repo().Role().Get(id, spaceId);
	return res.ExtractValue();
}

formats::json::Value Role::Delete(
	const server::http::HttpRequest& req,
	formats::json::ValueBuilder& res,
	const formats::json::Value& params) const
{
	const auto isAdmin = boost::lexical_cast<bool>(params[headers::kSpaceIsAdmin].As<std::string>());
	if (!isAdmin)
		throw errors::Forbidden403();

	const auto spaceId = params[headers::kSpaceId].As<boost::uuids::uuid>();
	const auto id = params["id"].As<int>();
	_s.Repo().Role().Delete(id, spaceId);

	req.SetResponseStatus(server::http::HttpStatus::kNoContent);
	return res.ExtractValue();
}

formats::json::Value Role::Post(
	const server::http::HttpRequest& req,
	const formats::json::Value& body,
	formats::json::ValueBuilder& res,
	const formats::json::Value& params) const
{
	const auto isAdmin = boost::lexical_cast<bool>(params[headers::kSpaceIsAdmin].As<std::string>());
	if (!isAdmin)
		throw errors::Forbidden403();

	auto item = model::Role{
		.id = 0,
		.spaceId = params[headers::kSpaceId].As<boost::uuids::uuid>(),
		.name = body["name"].As<std::string>()
	};

	_s.Repo().Role().Create(item);

	req.SetResponseStatus(server::http::HttpStatus::kCreated);
	return res.ExtractValue();
}

formats::json::Value Role::Put(
	const server::http::HttpRequest& req,
	const formats::json::Value& body,
	formats::json::ValueBuilder& res,
	const formats::json::Value& params) const
{
	const auto isAdmin = boost::lexical_cast<bool>(params[headers::kSpaceIsAdmin].As<std::string>());
	if (!isAdmin)
		throw errors::Forbidden403();

	auto item = body.As<model::Role>();
	item.spaceId = params[headers::kSpaceId].As<boost::uuids::uuid>();

	_s.Repo().Role().Update(item);

	req.SetResponseStatus(server::http::HttpStatus::kNoContent);
	return res.ExtractValue();
}

} // namespace svetit::space::handlers
