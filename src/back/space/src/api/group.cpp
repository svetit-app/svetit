#include "group.hpp"
#include "../model/group_serialize.hpp"
#include "../service/service.hpp"
#include "../repo/repository.hpp"
#include <shared/headers.hpp>
#include <shared/errors.hpp>
#include <shared/errors_catchit.hpp>
#include <shared/paging.hpp>
#include <shared/parse/request.hpp>
#include <shared/schemas.hpp>
#include <shared/parse/uuid.hpp>

#include <boost/lexical_cast.hpp>

namespace svetit::space::handlers {

Group::Group(
	const components::ComponentConfig& conf,
	const components::ComponentContext& ctx)
	: server::handlers::HttpHandlerJsonBase{conf, ctx}
	, _s{ctx.FindComponent<Service>()}
	, _mapHttpMethodToSchema{LoadSchemas(kName, _s.GetJSONSchemasPath())}
{}

formats::json::Value Group::HandleRequestJsonThrow(
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

formats::json::Value Group::Get(
	formats::json::ValueBuilder& res,
	const formats::json::Value& params) const
{
	const auto spaceId = params[headers::kSpaceId].As<boost::uuids::uuid>();
	const auto id = params["id"].As<int>();
	res = _s.Repo().Group().Get(id, spaceId);
	return res.ExtractValue();
}

formats::json::Value Group::Delete(
	const server::http::HttpRequest& req,
	formats::json::ValueBuilder& res,
	const formats::json::Value& params) const
{
	const auto isAdmin = boost::lexical_cast<bool>(params[headers::kSpaceIsAdmin].As<std::string>());
	if (!isAdmin)
		throw errors::Forbidden403();

	const auto spaceId = params[headers::kSpaceId].As<boost::uuids::uuid>();
	const auto id = params["id"].As<int>();

	_s.Repo().Group().Delete(id, spaceId);

	req.SetResponseStatus(server::http::HttpStatus::kNoContent);
	return res.ExtractValue();
}

formats::json::Value Group::Post(
	const server::http::HttpRequest& req,
	const formats::json::Value& body,
	formats::json::ValueBuilder& res,
	const formats::json::Value& params) const
{
	const auto isAdmin = boost::lexical_cast<bool>(params[headers::kSpaceIsAdmin].As<std::string>());
	if (!isAdmin)
		throw errors::Forbidden403();

	auto group = body.As<model::Group>();
	group.spaceId = params[headers::kSpaceId].As<boost::uuids::uuid>();

	_s.Repo().Group().Create(group);

	req.SetResponseStatus(server::http::HttpStatus::kCreated);
	return res.ExtractValue();
}

formats::json::Value Group::Put(
	const server::http::HttpRequest& req,
	const formats::json::Value& body,
	formats::json::ValueBuilder& res,
	const formats::json::Value& params) const
{
	const auto isAdmin = boost::lexical_cast<bool>(params[headers::kSpaceIsAdmin].As<std::string>());
	if (!isAdmin)
		throw errors::Forbidden403();

	auto group = body.As<model::Group>();
	group.spaceId = params[headers::kSpaceId].As<boost::uuids::uuid>();

	_s.Repo().Group().Update(group);

	req.SetResponseStatus(server::http::HttpStatus::kNoContent);
	return res.ExtractValue();
}

} // namespace svetit::space::handlers
