#include "invitation.hpp"
#include "../service/service.hpp"
#include "../../../shared/headers.hpp"
#include "../../../shared/errors.hpp"
#include "../../../shared/errors_catchit.hpp"
#include "../../../shared/paging.hpp"
#include "../../../shared/paging_serialize.hpp"
#include "../../../shared/parse/request.hpp"
#include "../model/role.hpp"
#include "../model/invitation_serialize.hpp"

namespace svetit::space::handlers {

Invitation::Invitation(
	const components::ComponentConfig& conf,
	const components::ComponentContext& ctx)
	: server::handlers::HttpHandlerJsonBase{conf, ctx}
	, _s{ctx.FindComponent<Service>()}
{}

formats::json::Value Invitation::HandleRequestJsonThrow(
	const server::http::HttpRequest& req,
	const formats::json::Value& body,
	server::request::RequestContext&) const
{
	formats::json::ValueBuilder res;

	const auto userId = req.GetHeader(headers::kUserId);
	if (userId.empty())
		throw errors::Unauthorized401{};

	try {
		switch (req.GetMethod()) {
		case server::http::HttpMethod::kGet:
			return GetList(req, res, userId);
		case server::http::HttpMethod::kPost:
			return Post(req, body, res, userId);
		case server::http::HttpMethod::kPut:
			return ChangeRole(req, body, res, userId);
		case server::http::HttpMethod::kPatch:
			return Join(req, res, userId);
		case server::http::HttpMethod::kDelete:
			return Delete(req, res, userId);
		default:
			throw std::runtime_error("Unsupported");
			break;
		}
	} catch(...) {
		return errors::CatchIt(req);
	}
	return res.ExtractValue();
}

formats::json::Value Invitation::GetList(
	const server::http::HttpRequest& req,
	formats::json::ValueBuilder& res,
	const std::string& userId) const
{
	auto paging = parsePaging(req);
	if (_s.IsListLimit(paging.limit))
		throw errors::BadRequest400("Too big limit param");

	if (req.HasArg("spaceId")) {
		const auto spaceId = parseUUID(req, "spaceId");
		res = _s.GetInvitationListBySpace(spaceId, paging.start, paging.limit, userId);
		return res.ExtractValue();
	}

	res = _s.GetInvitationList(paging.start, paging.limit, userId);

	return res.ExtractValue();
}

formats::json::Value Invitation::Post(
	const server::http::HttpRequest& req,
	const formats::json::Value& body,
	formats::json::ValueBuilder& res,
	const std::string& userId) const
{
	if (req.HasArg("link")) {
		const auto link = parseUUID(req, "link");

		if (!_s.InviteByLink(userId, link))
			throw errors::BadRequest400{"Link expired"};

		req.SetResponseStatus(server::http::HttpStatus::kCreated);
		return res.ExtractValue();
	}

	auto invitation = body.As<model::SpaceInvitation>();

	if (invitation.spaceId.is_nil() || invitation.userId.empty())
		throw errors::BadRequest400{"Params must be set"};

	_s.Invite(userId, invitation.spaceId, invitation.userId, invitation.role);

	req.SetResponseStatus(server::http::HttpStatus::kCreated);
	return res.ExtractValue();
}

formats::json::Value Invitation::ChangeRole(
	const server::http::HttpRequest& req,
	const formats::json::Value& body,
	formats::json::ValueBuilder& res,
	const std::string& userId) const
{
	const auto id = parsePositiveInt(req, "id");

	if (!body.HasMember("role"))
		throw errors::BadRequest400{"No role param in body"};

	const auto role = Role::FromString(body["role"].As<std::string>());

	_s.ChangeRoleInInvitation(id, role, userId);

	return res.ExtractValue();
}

formats::json::Value Invitation::Join(
	const server::http::HttpRequest& req,
	formats::json::ValueBuilder& res,
	const std::string& userId) const
{
	const auto id = parsePositiveInt(req, "id");

	_s.ApproveInvitation(id, userId);

	return res.ExtractValue();
}

formats::json::Value Invitation::Delete(
	const server::http::HttpRequest& req,
	formats::json::ValueBuilder& res,
	const std::string& userId) const
{
	const auto id = parsePositiveInt(req, "id");
	_s.DeleteInvitation(id, userId);

	return res.ExtractValue();
}

} // namespace svetit::space::handlers