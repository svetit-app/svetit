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
			LOG_WARNING() << "Wrong start query param: " << e.what() << " , start=" << start;
			req.SetResponseStatus(server::http::HttpStatus::kBadRequest);
			res["err"] = "Wrong start query param";
			return res.ExtractValue();
		}

		try {
			iLimit = boost::lexical_cast<int>(limit);
		} catch(const std::exception& e) {
			LOG_WARNING() << "Wrong limit query param: " << e.what() << " , limit=" << limit;
			req.SetResponseStatus(server::http::HttpStatus::kBadRequest);
			res["err"] = "Wrong limit query param";
			return res.ExtractValue();
		}

		if (iStart < 0) {
			LOG_WARNING() << "Start param must be unsigned int, gotten=" << iStart;
			req.SetResponseStatus(server::http::HttpStatus::kBadRequest);
			res["err"] = "Start param must be unsigned int";
			return res.ExtractValue();
		}

		if (iLimit <= 0) {
			LOG_WARNING() << "Limit param must be more then 0, gotten=" << iLimit;
			req.SetResponseStatus(server::http::HttpStatus::kBadRequest);
			res["err"] = "Limit param must be more then 0";
			return res.ExtractValue();
		}
	} else {
		LOG_WARNING() << "Start and Limit params must be set, gotten start=" << start << " limit=" << limit;
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

	// todo - need to validate that this is valid uuid? need to check that user exists?
	const auto& creatorId = req.GetHeader(headers::kUserId);
	if (creatorId.empty()) {
		res["err"] = "Empty userId header";
		req.SetResponseStatus(server::http::HttpStatus::kUnauthorized);
		return res.ExtractValue();
	}

	if (!body.HasMember("spaceId")) {
		LOG_WARNING() << "No spaceId param in body";
		req.SetResponseStatus(server::http::HttpStatus::kBadRequest);
		res["err"] = "No spaceId param in body";
		return res.ExtractValue();
	}

	if (!body.HasMember("userId")) {
		LOG_WARNING() << "No userId param in body";
		req.SetResponseStatus(server::http::HttpStatus::kBadRequest);
		res["err"] = "No userId param in body";
		return res.ExtractValue();
	}

	if (!body.HasMember("role")) {
		LOG_WARNING() << "No role param in body";
		req.SetResponseStatus(server::http::HttpStatus::kBadRequest);
		res["err"] = "No role param in body";
		return res.ExtractValue();
	}

	const auto spaceId = body["spaceId"].ConvertTo<std::string>();
	const auto userId = body["userId"].ConvertTo<std::string>();
	const auto role = body["role"].ConvertTo<std::string>();

	if (spaceId.empty() || userId.empty() || role.empty()) {
		LOG_WARNING() << "SpaceId, UserId, Role must be set";
		req.SetResponseStatus(server::http::HttpStatus::kBadRequest);
		res["err"] = "SpaceId, UserId, Role must be set";
		return res.ExtractValue();
	}

	if (!_s.ValidateUUID(spaceId)){
		LOG_WARNING() << "SpaceId must be valid uuid";
		req.SetResponseStatus(server::http::HttpStatus::kBadRequest);
		res["err"] = "SpaceId must be valid uuid";
		return res.ExtractValue();
	}

	if (!_s.ValidateUUID(userId)){
		LOG_WARNING() << "UserId must be valid uuid";
		req.SetResponseStatus(server::http::HttpStatus::kBadRequest);
		res["err"] = "UserId must be valid uuid";
		return res.ExtractValue();
	}

	if (!_s.ValidateRole(role)) {
		LOG_WARNING() << "Wrong role";
		req.SetResponseStatus(server::http::HttpStatus::kBadRequest);
		res["err"] = "Wrong role";
		return res.ExtractValue();
	}

	try {
		std::string msg;
		if (_s.Invite(creatorId, spaceId, userId, role, msg)) {
			req.SetResponseStatus(server::http::HttpStatus::kCreated);
		} else {
			LOG_WARNING() << msg;
			req.SetResponseStatus(server::http::HttpStatus::kBadRequest);
			res["err"] = msg;
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