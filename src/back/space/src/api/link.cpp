#include "link.hpp"
#include "../model/link_serialize.hpp"
#include "../service/service.hpp"
#include <svetit/headers.hpp>
#include <svetit/errors.hpp>
#include <svetit/errors_catchit.hpp>
#include <svetit/paging.hpp>
#include <svetit/paging_serialize.hpp>
#include <svetit/parse/request.hpp>
#include <svetit/schemas.hpp>
#include <svetit/parse/uuid.hpp>

namespace svetit::space::handlers {

Link::Link(
	const components::ComponentConfig& conf,
	const components::ComponentContext& ctx)
	: server::handlers::HttpHandlerJsonBase{conf, ctx}
	, _s{ctx.FindComponent<Service>()}
	, _mapHttpMethodToSchema{LoadSchemas(kName, _s.GetJSONSchemasPath())}
{}


formats::json::Value Link::HandleRequestJsonThrow(
	const server::http::HttpRequest& req,
	const formats::json::Value& body,
	server::request::RequestContext&) const
{
	formats::json::ValueBuilder res;

	try {
		const auto params = ValidateRequest(_mapHttpMethodToSchema, req, body);
		const auto userId = params[headers::kUserId].As<std::string>();

		switch (req.GetMethod()) {
			case server::http::HttpMethod::kGet:
				return GetList(res, userId, params);
			case server::http::HttpMethod::kPost:
				return Post(req, res, userId, params);
			case server::http::HttpMethod::kPut:
				return Put(req, body, res, userId);
			case server::http::HttpMethod::kDelete:
				return Delete(req, res, userId, params);
			default:
				throw std::runtime_error("Unsupported");
				break;
		}
	} catch(...) {
		return errors::CatchIt(req);
	}

	return res.ExtractValue();
}

formats::json::Value Link::GetList(
	formats::json::ValueBuilder& res,
	const std::string& userId,
	const formats::json::Value& params) const
{
	const auto paging = parsePaging(params);

	const auto spaceId = params["spaceId"].As<boost::uuids::uuid>(boost::uuids::uuid{});
	if (!spaceId.is_nil()) {
		res = _s.GetLinkListBySpace(spaceId, paging.start, paging.limit, userId);
		return res.ExtractValue();
	}

	res = _s.GetLinkList(paging.start, paging.limit, userId);
	return res.ExtractValue();
}

formats::json::Value Link::Post(
	const server::http::HttpRequest& req,
	formats::json::ValueBuilder& res,
	const std::string& userId,
	const formats::json::Value& params) const
{
	const auto link = params["id"].As<boost::uuids::uuid>();

	if (!_s.InviteByLink(userId, link))
		throw errors::BadRequest400{"Link expired"};

	req.SetResponseStatus(server::http::HttpStatus::kCreated);
	return res.ExtractValue();
}

formats::json::Value Link::Put(
	const server::http::HttpRequest& req,
	const formats::json::Value& body,
	formats::json::ValueBuilder& res,
	const std::string& userId) const
{
	const auto link = body.As<model::SpaceLink>();
	if (!_s.CheckExpiredAtValidity(link.expiredAt))
		throw errors::BadRequest400{"Wrong expiredAt"};

	_s.CreateInvitationLink(link.spaceId, userId, link.name, link.expiredAt);

	req.SetResponseStatus(server::http::HttpStatus::kCreated);
	return res.ExtractValue();
}

formats::json::Value Link::Delete(
	const server::http::HttpRequest& req,
	formats::json::ValueBuilder& res,
	const std::string& userId,
	const formats::json::Value& params) const
{
	const auto id = params["id"].As<boost::uuids::uuid>();

	_s.DeleteInvitationLink(id, userId);

	req.SetResponseStatus(server::http::HttpStatus::kNoContent);
	return res.ExtractValue();
}

} // namespace svetit::space::handlers
