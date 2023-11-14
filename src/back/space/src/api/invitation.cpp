#include "invitation.hpp"
#include "../service/service.hpp"
#include "../../../shared/headers.hpp"
#include "../../../shared/errors.hpp"
#include "../model/role.hpp"

namespace svetit::space::handlers {

Invitation::Invitation(
	const components::ComponentConfig& conf,
	const components::ComponentContext& ctx)
	: server::handlers::HttpHandlerJsonBase{conf, ctx}
	, _s{ctx.FindComponent<Service>()}
{}

formats::json::Value Invitation::GetList(
	const server::http::HttpRequest& req,
	const formats::json::Value& body) const
{
	formats::json::ValueBuilder res;

	try {
		auto paging = parsePaging(req);
		res["list"] = _s.GetInvitationList(paging.start, paging.limit);
		res["total"] = _s.GetInvitationsCount();
	}
	catch(const errors::BadRequest& e) {
		res["err"] = e.what();
		req.SetResponseStatus(server::http::HttpStatus::kBadRequest);
		return res.ExtractValue();
	}
	catch(const std::exception& e) {
		LOG_WARNING() << "Fail to get invitations list: " << e.what();
		req.SetResponseStatus(server::http::HttpStatus::kInternalServerError);
		res["err"] = "Fail to get invitations list";
	}

	return res.ExtractValue();
}

formats::json::Value Invitation::Post(
	const server::http::HttpRequest& req,
	const formats::json::Value& body) const
{
	formats::json::ValueBuilder res;

	const auto& creatorId = req.GetHeader(headers::kUserId);
	if (creatorId.empty()) {
		res["err"] = "Access denied";
		req.SetResponseStatus(server::http::HttpStatus::kUnauthorized);
		return res.ExtractValue();
	}

	const auto link = req.GetArg("link");
	bool linkMode = false;
	model::SpaceInvitation invitation;

	if (req.HasArg("link")) {
		if (!link.empty()) {
			linkMode = true;
		} else {
			req.SetResponseStatus(server::http::HttpStatus::kBadRequest);
			res["err"] = "Link must not be empty";
			return res.ExtractValue();
		}
	} else {
		invitation = body.As<model::SpaceInvitation>();

		if (invitation.spaceId.is_nil() || invitation.userId.empty()) {
			req.SetResponseStatus(server::http::HttpStatus::kBadRequest);
			res["err"] = "Params must be set";
			return res.ExtractValue();
		}

		if (!_s.ValidateRole(invitation.role)) {
			req.SetResponseStatus(server::http::HttpStatus::kBadRequest);
			res["err"] = "Wrong role";
			return res.ExtractValue();
		}
	}

	try {
		if (linkMode) {
			if (_s.InviteByLink(creatorId, link)) {
				req.SetResponseStatus(server::http::HttpStatus::kCreated);
			} else {
				req.SetResponseStatus(server::http::HttpStatus::kBadRequest);
				res["err"] = "Wrong link";
			}
		} else {
			if (_s.Invite(creatorId, invitation.spaceId, invitation.userId, invitation.role)) {
				req.SetResponseStatus(server::http::HttpStatus::kCreated);
			} else {
				req.SetResponseStatus(server::http::HttpStatus::kBadRequest);
				res["err"] = "Can't create invite";
			}
		}
	} catch(errors::BadRequest& e) {
		req.SetResponseStatus(server::http::HttpStatus::kBadRequest);
		res["err"] = e.what();
		return res.ExtractValue();
	} catch(errors::NotFound& e) {
		req.SetResponseStatus(server::http::HttpStatus::kNotFound);
		res["err"] = e.what();
		return res.ExtractValue();
	} catch(const std::exception& e) {
		LOG_WARNING() << "Fail to create invitation: " << e.what();
		res["err"] = "Fail to create invitation";
		req.SetResponseStatus(server::http::HttpStatus::kInternalServerError);
	}

	return res.ExtractValue();
}

formats::json::Value Invitation::HandleRequestJsonThrow(
	const server::http::HttpRequest& req,
	const formats::json::Value& body,
	server::request::RequestContext&) const
{
	switch (req.GetMethod()) {
		case server::http::HttpMethod::kGet:
			return GetList(req, body);
		case server::http::HttpMethod::kPost:
			return Post(req, body);
		case server::http::HttpMethod::kPut:
			return ChangeRole(req, body);
		case server::http::HttpMethod::kPatch:
			return Join(req, body);
		case server::http::HttpMethod::kDelete:
			return Delete(req, body);
		default: break;
	}

	formats::json::ValueBuilder res;
	res["err"] = "Unsupported";
	req.SetResponseStatus(server::http::HttpStatus::kInternalServerError);
	return res.ExtractValue();
}

formats::json::Value Invitation::ChangeRole(
	const server::http::HttpRequest& req,
	const formats::json::Value& body) const
{
	formats::json::ValueBuilder res;

	const auto& id = req.GetArg("id");

	if (id.empty()) {
		req.SetResponseStatus(server::http::HttpStatus::kBadRequest);
		res["err"] = "Param id must be set";
		return res.ExtractValue();
	}

	int iId;

	try {
		iId = boost::lexical_cast<int>(id);
	} catch(const std::exception& e) {
		req.SetResponseStatus(server::http::HttpStatus::kBadRequest);
		res["err"] = "Id param must be valid";
		return res.ExtractValue();
	}

	if (iId < 0) {
		req.SetResponseStatus(server::http::HttpStatus::kBadRequest);
		res["err"] = "Id param must be valid";
		return res.ExtractValue();
	}

	if (!body.HasMember("role")) {
		req.SetResponseStatus(server::http::HttpStatus::kBadRequest);
		res["err"] = "No role param in body";
		return res.ExtractValue();
	}

	const auto role = Role::FromString(body["role"].As<std::string>());

	if (!_s.ValidateRole(role)) {
		req.SetResponseStatus(server::http::HttpStatus::kBadRequest);
		res["err"] = "Wrong role";
		return res.ExtractValue();
	}

	try {
		if (!_s.ChangeRoleInInvitation(iId, role)){
			req.SetResponseStatus(server::http::HttpStatus::kNotModified);
		}
	}
	catch(const std::exception& e) {
		LOG_WARNING() << "Fail to change role in invitation: " << e.what();
		res["err"] = "Fail to change role in invitation";
		req.SetResponseStatus(server::http::HttpStatus::kInternalServerError);
	}

	return res.ExtractValue();
}

formats::json::Value Invitation::Join(
	const server::http::HttpRequest& req,
	const formats::json::Value& body) const
{
	formats::json::ValueBuilder res;

	const auto& id = req.GetArg("id");

	if (id.empty()) {
		req.SetResponseStatus(server::http::HttpStatus::kBadRequest);
		res["err"] = "Param id must be set";
		return res.ExtractValue();
	}

	int iId;

	try {
		iId = boost::lexical_cast<int>(id);
	} catch(const std::exception& e) {
		req.SetResponseStatus(server::http::HttpStatus::kBadRequest);
		res["err"] = "Id param must be valid";
		return res.ExtractValue();
	}

	if (iId < 0) {
		req.SetResponseStatus(server::http::HttpStatus::kBadRequest);
		res["err"] = "Id param must be valid";
		return res.ExtractValue();
	}

	try {
		if (!_s.ApproveInvitation(iId)) {
			req.SetResponseStatus(server::http::HttpStatus::kNotModified);
		}
	} catch(errors::BadRequest& e) {
		req.SetResponseStatus(server::http::HttpStatus::kBadRequest);
		res["err"] = e.what();
		return res.ExtractValue();
	} catch(errors::NotFound& e) {
		req.SetResponseStatus(server::http::HttpStatus::kNotFound);
		res["err"] = e.what();
		return res.ExtractValue();
	} catch(const std::exception& e) {
		LOG_WARNING() << "Fail to approve invitation: " << e.what();
		res["err"] = "Fail to approve invitation";
		req.SetResponseStatus(server::http::HttpStatus::kInternalServerError);
	}

	return res.ExtractValue();
}

formats::json::Value Invitation::Delete(
	const server::http::HttpRequest& req,
	const formats::json::Value& body) const
{
	formats::json::ValueBuilder res;

		const auto& id = req.GetArg("id");

	if (id.empty()) {
		req.SetResponseStatus(server::http::HttpStatus::kBadRequest);
		res["err"] = "Param id must be set";
		return res.ExtractValue();
	}

	int iId;

	try {
		iId = boost::lexical_cast<int>(id);
	} catch(const std::exception& e) {
		req.SetResponseStatus(server::http::HttpStatus::kBadRequest);
		res["err"] = "Id param must be valid";
		return res.ExtractValue();
	}

	if (iId < 0) {
		req.SetResponseStatus(server::http::HttpStatus::kBadRequest);
		res["err"] = "Id param must be valid";
		return res.ExtractValue();
	}

	try {
		if (!_s.DeleteInvitation(iId)) {
			req.SetResponseStatus(server::http::HttpStatus::kNotFound);
		}
	}
	catch(const std::exception& e) {
		LOG_WARNING() << "Fail to delete invitation: " << e.what();
		res["err"] = "Fail to delete invitation";
		req.SetResponseStatus(server::http::HttpStatus::kInternalServerError);
	}

	return res.ExtractValue();
}

} // namespace svetit::space::handlers