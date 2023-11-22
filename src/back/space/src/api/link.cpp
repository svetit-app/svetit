#include "link.hpp"
#include "../service/service.hpp"
#include "../../../shared/headers.hpp"
#include "../../../shared/errors.hpp"
#include "../../../shared/paging.hpp"
#include "../../../shared/paging.сpp"
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

	try {
		switch (req.GetMethod()) {
			case server::http::HttpMethod::kGet:
				return GetList(req, res);
			case server::http::HttpMethod::kPost:
				return Post(req, body, res);
			case server::http::HttpMethod::kDelete:
				return Delete(req, res);
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
	formats::json::ValueBuilder& res) const
{
	auto paging = parsePaging(req);
	res["list"] = _s.GetLinkList(paging.start, paging.limit);
	res["total"] = _s.GetLinksCount();

	return res.ExtractValue();
}

formats::json::Value Link::Post(
	const server::http::HttpRequest& req,
	const formats::json::Value& body,
	formats::json::ValueBuilder& res) const
{
	const auto& creatorId = req.GetHeader(headers::kUserId);
	if (creatorId.empty())
		throw errors::Unauthorized{};

	const auto link = body.As<model::SpaceLink>();

	if (!_s.CheckExpiredAtValidity(link.expiredAt))
		throw errors::BadRequest{"Wrong expiredAt"};

	if (link.spaceId.is_nil() || link.name.empty())
		throw errors::BadRequest{"Params must be set"};

	_s.CreateInvitationLink(link.spaceId, creatorId, link.name, link.expiredAt);

	return res.ExtractValue();
}

formats::json::Value Link::Delete(
	const server::http::HttpRequest& req,
	formats::json::ValueBuilder& res) const
{
	const auto& id = req.GetArg("id");
	if (id.empty())
		throw errors::BadRequest{"Param id must be set"};

	_s.DeleteInvitationLink(id);

	return res.ExtractValue();
}

} // namespace svetit::space::handlers