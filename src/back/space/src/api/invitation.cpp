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
		auto paging = paging::parsePaging(req);
		res["list"] = _s.GetInvitationList(paging.start, paging.limit);
		res["total"] = _s.GetInvitationsCount();
	}
	catch(const errors::BadRequestException& e) {
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

	std::string spaceId;
	std::string userId;
	Role::Type role;

	const auto link = req.GetArg("link");
	bool linkMode = false;

	if (req.HasArg("link")) {
		if (!link.empty()) {
			linkMode = true;
		} else {
			req.SetResponseStatus(server::http::HttpStatus::kBadRequest);
			res["err"] = "Link must not be empty";
			return res.ExtractValue();
		}
	} else {
		if (!body.HasMember("spaceId")) {
			req.SetResponseStatus(server::http::HttpStatus::kBadRequest);
			res["err"] = "No spaceId param in body";
			return res.ExtractValue();
		}

		if (!body.HasMember("userId")) {
			req.SetResponseStatus(server::http::HttpStatus::kBadRequest);
			res["err"] = "No userId param in body";
			return res.ExtractValue();
		}

		if (!body.HasMember("role")) {
			req.SetResponseStatus(server::http::HttpStatus::kBadRequest);
			res["err"] = "No role param in body";
			return res.ExtractValue();
		}

		spaceId = body["spaceId"].ConvertTo<std::string>();
		userId = body["userId"].ConvertTo<std::string>();
		const auto roleStr = body["role"].ConvertTo<std::string>();
		role = Role::FromString(body["role"].As<std::string>());


		if (spaceId.empty() || userId.empty() || roleStr.empty()) {
			req.SetResponseStatus(server::http::HttpStatus::kBadRequest);
			res["err"] = "Params must be set";
			return res.ExtractValue();
		}

		if (!_s.ValidateRole(role)) {
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
			if (_s.Invite(creatorId, spaceId, userId, role)) {
				req.SetResponseStatus(server::http::HttpStatus::kCreated);
			} else {
				req.SetResponseStatus(server::http::HttpStatus::kBadRequest);
				res["err"] = "Can't create invite";
			}
		}
	} catch(errors::BadRequestException& e) {
		req.SetResponseStatus(server::http::HttpStatus::kBadRequest);
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
	}
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
	} catch(errors::BadRequestException& e) {
		req.SetResponseStatus(server::http::HttpStatus::kBadRequest);
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