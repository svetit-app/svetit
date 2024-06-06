#include "space.hpp"
#include "../model/space_serialize.hpp"
#include "../service/service.hpp"
#include <shared/headers.hpp>
#include <shared/errors.hpp>
#include <shared/errors_catchit.hpp>
#include <shared/paging.hpp>
#include <shared/parse/request.hpp>
#include <shared/schemas.hpp>
#include <shared/parse/uuid.hpp>

namespace svetit::space::handlers {

Space::Space(
	const components::ComponentConfig& conf,
	const components::ComponentContext& ctx)
	: server::handlers::HttpHandlerJsonBase{conf, ctx}
	, _s{ctx.FindComponent<Service>()}
	, _mapHttpMethodToSchema{LoadSchemas(kName, _s.GetJSONSchemasPath())}
{}

formats::json::Value Space::HandleRequestJsonThrow(
	const server::http::HttpRequest& req,
	const formats::json::Value& body,
	server::request::RequestContext&) const
{
	formats::json::ValueBuilder res;

	try {
		const auto params = ValidateRequest(_mapHttpMethodToSchema, req, body);

		switch (req.GetMethod()) {
			case server::http::HttpMethod::kGet:
				return Get(req, res, params);
			case server::http::HttpMethod::kPost:
				return Post(req, body, res, params);
			case server::http::HttpMethod::kDelete:
				return Delete(req, res, params);
			case server::http::HttpMethod::kHead:
				return Head(req, res, params);
			default:
				throw std::runtime_error("Unsupported");
				break;
		}
	} catch(...) {
		return errors::CatchIt(req);
	}

	return res.ExtractValue();
}

formats::json::Value Space::Get(
	const server::http::HttpRequest& req,
	formats::json::ValueBuilder& res,
	const formats::json::Value& params) const
{
	if (params.HasMember("id"))
	{
		const auto userId = params[headers::kUserId].As<std::string>();

		const auto id = params["id"].As<boost::uuids::uuid>();
		res = _s.GetById(id, userId);
	}
	else if (params.HasMember("key"))
	{
		const auto userId = params[headers::kUserId].As<std::string>();

		const auto key = params["key"].As<std::string>();
		if (!_s.KeyWeakCheck(key))
			throw errors::BadRequest400{"Key must be valid"};
		res = _s.GetByKey(key, userId);
	}
	else if (params.HasMember("link"))
	{
		const auto linkId = params["link"].As<boost::uuids::uuid>();
		res = _s.GetByLink(linkId);
	}
	else
		throw errors::BadRequest400{"No arguments"};

	return res.ExtractValue();
}

formats::json::Value Space::Delete(
	const server::http::HttpRequest& req,
	formats::json::ValueBuilder& res,
	const formats::json::Value& params) const
{
	const auto userId = params[headers::kUserId].As<std::string>();

	const auto id = params["id"].As<boost::uuids::uuid>();
	if (!_s.IsSpaceOwner(id, userId))
		throw errors::NotFound404();

	_s.Delete(id);

	return res.ExtractValue();
}

formats::json::Value Space::Post(
	const server::http::HttpRequest& req,
	const formats::json::Value& body,
	formats::json::ValueBuilder& res,
	const formats::json::Value& params) const
{
	const auto userId = params[headers::kUserId].As<std::string>();

	auto space = body.As<model::Space>();

	if (space.name.empty())
		throw errors::BadRequest400("Empty name");
	if (!_s.KeyCreateCheck(space.key, userId))
		throw errors::BadRequest400("Can't use such key");
	if (_s.isSpaceExistsByKey(space.key))
		throw errors::Conflict409{"Invalid key"};
	if (!_s.IsUserTimeouted(userId))
		throw std::runtime_error("Timeout");
	if (_s.IsLimitReached(userId))
		throw std::runtime_error("Limit");

	_s.Create(space.name, space.key, space.requestsAllowed, userId);

	req.SetResponseStatus(server::http::HttpStatus::kCreated);
	return res.ExtractValue();
}

formats::json::Value Space::Head(
	const server::http::HttpRequest& req,
	formats::json::ValueBuilder& res,
	const formats::json::Value& params) const
{
	const auto key = params["key"].As<std::string>();

	if (!_s.isSpaceExistsByKey(key))
		throw errors::NotFound404{};

	return res.ExtractValue();
}

} // namespace svetit::space::handlers
