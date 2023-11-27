#include "invitation.hpp"
#include "../service/service.hpp"
#include "../../../shared/headers.hpp"
#include "../../../shared/errors.hpp"
#include "../../../shared/paging.hpp"
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

	try {
		switch (req.GetMethod()) {
		case server::http::HttpMethod::kGet:
			return GetList(req, res);
		case server::http::HttpMethod::kPost:
			return Post(req, body, res);
		case server::http::HttpMethod::kPut:
			return ChangeRole(req, body, res);
		case server::http::HttpMethod::kPatch:
			return Join(req, res);
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
	} catch(const errors::NotModified& e) {
		req.SetResponseStatus(server::http::HttpStatus::kNotModified);
		res["err"] = e.what();
		return res.ExtractValue();
	} catch(const std::exception& e) {
		LOG_WARNING() << "Fail to process invitation handle with method: "
			<< req.GetMethodStr() << " err: " << e.what();
		res["err"] = "Fail to process invitation";
		req.SetResponseStatus(server::http::HttpStatus::kInternalServerError);
	}
	return res.ExtractValue();
}

formats::json::Value Invitation::GetList(
	const server::http::HttpRequest& req,
	formats::json::ValueBuilder& res) const
{
	auto paging = parsePaging(req);
	if (_s.IsListLimit(paging.limit))
		throw errors::BadRequest("Too big limit param");
	const auto list = _s.GetInvitationList(paging.start, paging.limit);
	res["list"] = list.items;
	res["total"] = list.total;
	return res.ExtractValue();
}

formats::json::Value Invitation::Post(
	const server::http::HttpRequest& req,
	const formats::json::Value& body,
	formats::json::ValueBuilder& res) const
{
	const auto creatorId = req.GetHeader(headers::kUserId);
	if (creatorId.empty())
		throw errors::Unauthorized{};

	if (req.HasArg("link")) {
		const auto link = parseUUID(req, "link");
		if (_s.IsLinkExpired(link))
			throw errors::BadRequest{"Link expired"};

		_s.InviteByLink(creatorId, link);

		req.SetResponseStatus(server::http::HttpStatus::kCreated);
		return res.ExtractValue();
	}

	auto invitation = body.As<model::SpaceInvitation>();

	if (invitation.spaceId.is_nil() || invitation.userId.empty())
		throw errors::BadRequest{"Params must be set"};
	if (!_s.ValidateRole(invitation.role))
		throw errors::BadRequest{"Wrong role"};

	_s.Invite(creatorId, invitation.spaceId, invitation.userId, invitation.role);

	req.SetResponseStatus(server::http::HttpStatus::kCreated);
	return res.ExtractValue();
}

formats::json::Value Invitation::ChangeRole(
	const server::http::HttpRequest& req,
	const formats::json::Value& body,
	formats::json::ValueBuilder& res) const
{
	const auto id = parsePositiveInt(req, "id");

	if (!body.HasMember("role"))
		throw errors::BadRequest{"No role param in body"};

	const auto role = Role::FromString(body["role"].As<std::string>());
	if (!_s.ValidateRole(role))
		throw errors::BadRequest{"Wrong role"};

	_s.ChangeRoleInInvitation(id, role);

	return res.ExtractValue();
}

formats::json::Value Invitation::Join(
	const server::http::HttpRequest& req,
	formats::json::ValueBuilder& res) const
{
	const auto id = parsePositiveInt(req, "id");

	_s.ApproveInvitation(id);

	return res.ExtractValue();
}

formats::json::Value Invitation::Delete(
	const server::http::HttpRequest& req,
	formats::json::ValueBuilder& res) const
{
	const auto id = parsePositiveInt(req, "id");
	_s.DeleteInvitation(id);

	return res.ExtractValue();
}

} // namespace svetit::space::handlers