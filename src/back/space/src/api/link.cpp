#include "link.hpp"
#include "../service/service.hpp"
#include "../../../shared/headers.hpp"
#include "../../../shared/errors.hpp"
#include "../../../shared/paging.hpp"
#include "../../../shared/paging_serialize.hpp"
#include "../../../shared/parse/request.hpp"
#include "../model/link_serialize.hpp"

namespace svetit::space::handlers {

Link::Link(
	const components::ComponentConfig& conf,
	const components::ComponentContext& ctx)
	: server::handlers::HttpHandlerJsonBase{conf, ctx}
	, _s{ctx.FindComponent<Service>()}
{}


formats::json::Value Link::HandleRequestJsonThrow(
	const server::http::HttpRequest& req,
	const formats::json::Value& body,
	server::request::RequestContext&) const
{
	formats::json::ValueBuilder res;

	const auto userId = req.GetHeader(headers::kUserId);
	if (userId.empty())
		throw errors::Unauthorized{};

	try {
		switch (req.GetMethod()) {
			case server::http::HttpMethod::kGet:
				return GetList(req, res, userId);
			case server::http::HttpMethod::kPost:
				return Post(req, body, res, userId);
			case server::http::HttpMethod::kDelete:
				return Delete(req, res, userId);
			default:
				throw std::runtime_error("Unsupported");
				break;
		}
	} catch(const errors::Unauthorized& e) {
		req.SetResponseStatus(server::http::HttpStatus::kUnauthorized);
		res["err"] = e.what();
		return res.ExtractValue();
	} catch(const errors::BadRequest& e) {
		req.SetResponseStatus(server::http::HttpStatus::kBadRequest);
		res["err"] = e.what();
		return res.ExtractValue();
	} catch(const errors::NotFound& e) {
		req.SetResponseStatus(server::http::HttpStatus::kNotFound);
		res["err"] = e.what();
		return res.ExtractValue();
	} catch(const std::exception& e) {
		LOG_WARNING() << "Fail to process link handle with method: "
			<< req.GetMethodStr() << " err: " << e.what();
		res["err"] = "Fail to process link";
		req.SetResponseStatus(server::http::HttpStatus::kInternalServerError);
	}

	return res.ExtractValue();
}

formats::json::Value Link::GetList(
	const server::http::HttpRequest& req,
	formats::json::ValueBuilder& res,
	const std::string& userId) const
{
	auto paging = parsePaging(req);
	if (_s.IsListLimit(paging.limit))
		throw errors::BadRequest("Too big limit param");

	if (req.HasArg("spaceId")) {
		const auto spaceId = parseUUID(req, "spaceId");
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
		throw errors::BadRequest{"Wrong expiredAt"};
	if (link.spaceId.is_nil() || link.name.empty())
		throw errors::BadRequest{"Params must be set"};

	_s.CreateInvitationLink(link.spaceId, userId, link.name, link.expiredAt);

	return res.ExtractValue();
}

formats::json::Value Link::Delete(
	const server::http::HttpRequest& req,
	formats::json::ValueBuilder& res,
	const std::string& userId) const
{
	const auto id = parseUUID(req, "id");

	_s.DeleteInvitationLink(id, userId);

	return res.ExtractValue();
}

} // namespace svetit::space::handlers