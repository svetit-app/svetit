#include "invitation.hpp"
#include "../service/service.hpp"
#include "../../../shared/headers.hpp"

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

	const auto& start = req.GetArg("start");
	const auto& limit = req.GetArg("limit");
	int iStart;
	int iLimit;

	if (!start.empty() && !limit.empty()){
		try {
			iStart = boost::lexical_cast<int>(start);
		} catch(const std::exception& e) {
			req.SetResponseStatus(server::http::HttpStatus::kBadRequest);
			res["err"] = "Wrong start query param";
			return res.ExtractValue();
		}

		try {
			iLimit = boost::lexical_cast<int>(limit);
		} catch(const std::exception& e) {
			req.SetResponseStatus(server::http::HttpStatus::kBadRequest);
			res["err"] = "Wrong limit query param";
			return res.ExtractValue();
		}

		if (iStart < 0) {
			req.SetResponseStatus(server::http::HttpStatus::kBadRequest);
			res["err"] = "Start param must be valid";
			return res.ExtractValue();
		}

		if (iLimit <= 0) {
			req.SetResponseStatus(server::http::HttpStatus::kBadRequest);
			res["err"] = "Limit param must be valid";
			return res.ExtractValue();
		}
	} else {
		req.SetResponseStatus(server::http::HttpStatus::kBadRequest);
		res["err"] = "Start and Limit params must be set";
		return res.ExtractValue();
	}

	try {
		res["list"] = _s.GetInvitationList(iStart, iLimit);
		res["total"] = _s.GetInvitationsCount();
	}
	catch(const std::exception& e) {
		LOG_WARNING() << " Fail to get invitations list: " << e.what();
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
	std::string role;

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
		role = body["role"].ConvertTo<std::string>();

		if (spaceId.empty() || userId.empty() || role.empty()) {
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

	}
	catch(const std::exception& e) {
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
	}
}

} // namespace svetit::space::handlers