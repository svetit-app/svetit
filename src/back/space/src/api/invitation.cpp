#include <boost/lexical_cast.hpp>

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

	try {
		const auto& creatorId = req.GetHeader(headers::kUserId);
		if (creatorId.empty())
			throw errors::Unauthorized{"Access denied"};

		if (req.HasArg("link")) {
			const auto link = req.GetArg("link");

			if (link.empty())
				throw errors::BadRequest{"Empty linkId"};

			if (!_s.InviteByLink(creatorId, link))
				throw errors::BadRequest{"Failed invite by link"};

			req.SetResponseStatus(server::http::HttpStatus::kCreated);
			return res.ExtractValue();
		}

		auto invitation = body.As<model::SpaceInvitation>();

		if (invitation.spaceId.is_nil() || invitation.userId.empty())
			throw errors::BadRequest{"Params must be set"};

		if (!_s.ValidateRole(invitation.role))
			throw errors::BadRequest{"Wrong role"};

		if (!_s.Invite(creatorId, invitation.spaceId, invitation.userId, invitation.role))
			throw errors::BadRequest{"Failed to invite"};

		req.SetResponseStatus(server::http::HttpStatus::kCreated);
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

	try {
		const auto& id = req.GetArg("id");

		if (id.empty())
			throw errors::BadRequest{"Param id must be set"};

		int iId;

		try {
			iId = boost::lexical_cast<int>(id);
		} catch(const std::exception& e) {
			req.SetResponseStatus(server::http::HttpStatus::kBadRequest);
			res["err"] = "Id param must be valid";
			return res.ExtractValue();
		}

		if (iId < 0)
			throw errors::BadRequest{"Id param must be valid"};

		if (!body.HasMember("role"))
			throw errors::BadRequest{"No role param in body"};

		const auto role = Role::FromString(body["role"].As<std::string>());

		if (!_s.ValidateRole(role))
			throw errors::BadRequest{"Wrong role"};

		if (!_s.ChangeRoleInInvitation(iId, role)){
			req.SetResponseStatus(server::http::HttpStatus::kNotModified);
		}
	} catch(const errors::BadRequest& e) {
		req.SetResponseStatus(server::http::HttpStatus::kBadRequest);
		res["err"] = e.what();
		return res.ExtractValue();
	} catch(const std::exception& e) {
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

	try {
		const auto& id = req.GetArg("id");

		if (id.empty())
			throw errors::BadRequest{"Param id must be set"};

		int iId;

		try {
			iId = boost::lexical_cast<int>(id);
		} catch(const std::exception& e) {
			req.SetResponseStatus(server::http::HttpStatus::kBadRequest);
			res["err"] = "Id param must be valid";
			return res.ExtractValue();
		}

		if (iId < 0)
			throw errors::BadRequest{"Id param must be valid"};

		if (!_s.ApproveInvitation(iId)) {
			req.SetResponseStatus(server::http::HttpStatus::kNotModified);
		}
	} catch(const errors::BadRequest& e) {
		req.SetResponseStatus(server::http::HttpStatus::kBadRequest);
		res["err"] = e.what();
		return res.ExtractValue();
	} catch(const errors::NotFound& e) {
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

	try {
		const auto& id = req.GetArg("id");

		if (id.empty())
			throw errors::BadRequest{"Param id must be set"};

		int iId;

		try {
			iId = boost::lexical_cast<int>(id);
		} catch(const std::exception& e) {
			req.SetResponseStatus(server::http::HttpStatus::kBadRequest);
			res["err"] = "Id param must be valid";
			return res.ExtractValue();
		}

		if (iId < 0)
			throw errors::BadRequest{"Id param must be valid"};

		if (!_s.DeleteInvitation(iId))
			throw errors::NotFound{};
	} catch(const errors::BadRequest& e) {
		req.SetResponseStatus(server::http::HttpStatus::kBadRequest);
		res["err"] = e.what();
		return res.ExtractValue();
	} catch(const errors::NotFound& e) {
		req.SetResponseStatus(server::http::HttpStatus::kNotFound);
		res["err"] = e.what();
		return res.ExtractValue();
	} catch(const std::exception& e) {
		LOG_WARNING() << "Fail to delete invitation: " << e.what();
		res["err"] = "Fail to delete invitation";
		req.SetResponseStatus(server::http::HttpStatus::kInternalServerError);
	}

	return res.ExtractValue();
}

} // namespace svetit::space::handlers