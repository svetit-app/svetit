#include "invitation.hpp"
#include "../service/service.hpp"
#include "../../../shared/headers.hpp"
#include "../../../shared/errors.hpp"
#include "../../../shared/paging.hpp"
#include "../../../shared/paging.сpp"
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
	res["list"] = _s.GetInvitationList(paging.start, paging.limit);
	res["total"] = _s.GetInvitationsCount();

	return res.ExtractValue();
}

formats::json::Value Invitation::Post(
	const server::http::HttpRequest& req,
	const formats::json::Value& body,
	formats::json::ValueBuilder& res) const
{
	const auto& creatorId = req.GetHeader(headers::kUserId);
	if (creatorId.empty())
		throw errors::Unauthorized{};

	if (req.HasArg("link")) {
		const auto link = req.GetArg("link");

		if (link.empty())
			throw errors::BadRequest{"Empty linkId"};

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
	const auto& id = req.GetArg("id");

	if (id.empty())
		throw errors::BadRequest{"Param id must be set"};

	int iId;

	try {
		iId = std::atoi(id.c_str());

	} catch(const std::exception& e) {
		throw errors::BadRequest("Id param must be valid");
	}

	if (iId < 0)
		throw errors::BadRequest{"Id param must be valid"};

	if (!body.HasMember("role"))
		throw errors::BadRequest{"No role param in body"};

	const auto role = Role::FromString(body["role"].As<std::string>());

	if (!_s.ValidateRole(role))
		throw errors::BadRequest{"Wrong role"};

	_s.ChangeRoleInInvitation(iId, role);

	return res.ExtractValue();
}

formats::json::Value Invitation::Join(
	const server::http::HttpRequest& req,
	formats::json::ValueBuilder& res) const
{
	const auto& id = req.GetArg("id");

	if (id.empty())
		throw errors::BadRequest{"Param id must be set"};

	int iId;

	try {
		iId = std::atoi(id.c_str());
	} catch(const std::exception& e) {
		req.SetResponseStatus(server::http::HttpStatus::kBadRequest);
		res["err"] = "Id param must be valid";
		return res.ExtractValue();
	}

	if (iId < 0)
		throw errors::BadRequest{"Id param must be valid"};

	_s.ApproveInvitation(iId);

	return res.ExtractValue();
}

formats::json::Value Invitation::Delete(
	const server::http::HttpRequest& req,
	formats::json::ValueBuilder& res) const
{
	const auto& id = req.GetArg("id");

	if (id.empty())
		throw errors::BadRequest{"Param id must be set"};

	int iId;

	try {
		iId = std::atoi(id.c_str());
	} catch(const std::exception& e) {
		req.SetResponseStatus(server::http::HttpStatus::kBadRequest);
		res["err"] = "Id param must be valid";
		return res.ExtractValue();
	}

	if (iId < 0)
		throw errors::BadRequest{"Id param must be valid"};

	_s.DeleteInvitation(iId);

	return res.ExtractValue();
}

} // namespace svetit::space::handlers