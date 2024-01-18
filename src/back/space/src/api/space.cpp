#include "space.hpp"
#include "../service/service.hpp"
#include "../../../shared/headers.hpp"
#include "../../../shared/errors.hpp"
#include "../../../shared/errors_catchit.hpp"
#include "../../../shared/paging.hpp"
#include "../../../shared/parse/request.hpp"
#include "../model/space_serialize.hpp"

namespace svetit::space::handlers {

Space::Space(
	const components::ComponentConfig& conf,
	const components::ComponentContext& ctx)
	: server::handlers::HttpHandlerJsonBase{conf, ctx}
	, _s{ctx.FindComponent<Service>()}
{}

formats::json::Value Space::HandleRequestJsonThrow(
	const server::http::HttpRequest& req,
	const formats::json::Value& body,
	server::request::RequestContext&) const
{
	formats::json::ValueBuilder res;

	try {
		switch (req.GetMethod()) {
			case server::http::HttpMethod::kGet:
				return Get(req, res);
			case server::http::HttpMethod::kPost:
				return Post(req, body, res);
			case server::http::HttpMethod::kDelete:
				return Delete(req, res);
			case server::http::HttpMethod::kHead:
				return Head(req, res);
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
	formats::json::ValueBuilder& res) const
{
	if (req.HasArg("id"))
	{
		const auto userId = req.GetHeader(headers::kUserId);
		if (userId.empty())
			throw errors::Unauthorized401{};

		const auto id = parseUUID(req, "id");
		res = _s.GetById(id, userId);
	}
	else if (req.HasArg("key"))
	{
		const auto userId = req.GetHeader(headers::kUserId);
		if (userId.empty())
			throw errors::Unauthorized401{};

		const auto key = req.GetArg("key");
		if (!_s.KeyWeakCheck(key))
			throw errors::BadRequest400{"Key must be valid"};
		res = _s.GetByKey(key, userId);
	}
	else if (req.HasArg("link"))
	{
		const auto linkId = parseUUID(req, "link");
		res = _s.GetByLink(linkId);
	}
	else
		throw errors::BadRequest400{"No arguments"};

	return res.ExtractValue();
}

formats::json::Value Space::Delete(
	const server::http::HttpRequest& req,
	formats::json::ValueBuilder& res) const
{
	const auto userId = req.GetHeader(headers::kUserId);
	if (userId.empty())
		throw errors::Unauthorized401{};

	const auto id = parseUUID(req, "id");
	if (!_s.IsSpaceOwner(id, userId))
		throw errors::NotFound404();

	_s.Delete(id);

	return res.ExtractValue();
}

formats::json::Value Space::Post(
	const server::http::HttpRequest& req,
	const formats::json::Value& body,
	formats::json::ValueBuilder& res) const
{
	const auto userId = req.GetHeader(headers::kUserId);
	if (userId.empty())
		throw errors::Unauthorized401{};

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
	formats::json::ValueBuilder& res) const
{
	const auto key = req.GetArg("key");
	if (key.empty())
		throw errors::BadRequest400{"Key param must be set"};

	if (!_s.isSpaceExistsByKey(key))
		throw errors::NotFound404{};

	return res.ExtractValue();
}

} // namespace svetit::space::handlers