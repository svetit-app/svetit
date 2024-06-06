#include "link.hpp"
#include "../model/link_serialize.hpp"
#include "../service/service.hpp"
#include <shared/headers.hpp>
#include <shared/errors.hpp>
#include <shared/errors_catchit.hpp>
#include <shared/paging.hpp>
#include <shared/paging_serialize.hpp>
#include <shared/parse/request.hpp>
#include <shared/schemas.hpp>
#include <shared/parse/uuid.hpp>
#include <shared/type_utils.hpp>

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
				return GetList(req, res, userId, params);
			case server::http::HttpMethod::kPost:
				return Post(req, body, res, userId);
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
	const server::http::HttpRequest& req,
	formats::json::ValueBuilder& res,
	const std::string& userId,
	const formats::json::Value& params) const
{
	Paging paging = {
		.start = params["start"].As<int>(),
		.limit = params["limit"].As<int>()
	};
	if (_s.IsListLimit(paging.limit))
		throw errors::BadRequest400("Too big limit param");

	if (params.HasMember("spaceId")) {
		const auto spaceId = params["spaceId"].As<boost::uuids::uuid>();
		res = _s.GetLinkListBySpace(spaceId, paging.start, paging.limit, userId);
		return res.ExtractValue();
	}

	res = _s.GetLinkList(paging.start, paging.limit, userId);
	return res.ExtractValue();
}

formats::json::Value Link::Post(
	const server::http::HttpRequest& req,
	const formats::json::Value& body,
	formats::json::ValueBuilder& res,
	const std::string& userId) const
{
	const auto link = body.As<model::SpaceLink>();
	if (!_s.CheckExpiredAtValidity(link.expiredAt))
		throw errors::BadRequest400{"Wrong expiredAt"};
	if (link.spaceId.is_nil() || link.name.empty())
		throw errors::BadRequest400{"Params must be set"};

	_s.CreateInvitationLink(link.spaceId, userId, link.name, link.expiredAt);

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

	return res.ExtractValue();
}

} // namespace svetit::space::handlers
