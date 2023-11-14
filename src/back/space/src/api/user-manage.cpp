#include "user-manage.hpp"
#include "../service/service.hpp"
#include "../../../shared/headers.hpp"
#include "../../../shared/errors.hpp"

namespace svetit::space::handlers {

UserManage::UserManage(
	const components::ComponentConfig& conf,
	const components::ComponentContext& ctx)
	: server::handlers::HttpHandlerJsonBase{conf, ctx}
	, _s{ctx.FindComponent<Service>()}
{}

formats::json::Value UserManage::HandleRequestJsonThrow(
	const server::http::HttpRequest& req,
	const formats::json::Value& body,
	server::request::RequestContext&) const
{
	formats::json::ValueBuilder res;

	const auto& userId = req.GetHeader(headers::kUserId);
	if (userId.empty()) {
		res["err"] = "Access denied";
		req.SetResponseStatus(server::http::HttpStatus::kUnauthorized);
		return res.ExtractValue();
	}

	switch (req.GetMethod()) {
		case server::http::HttpMethod::kDelete:
			return Delete(userId, req, body);
		case server::http::HttpMethod::kPatch:
			return UpdateUser(userId, req, body);
		default: break;
	}

	res["err"] = "Unsupported";
	req.SetResponseStatus(server::http::HttpStatus::kInternalServerError);
	return res.ExtractValue();
}

formats::json::Value UserManage::Delete(
	const std::string headerUserId,
	const server::http::HttpRequest& req,
	const formats::json::Value& body) const
{
	formats::json::ValueBuilder res;

	const auto spaceId = req.GetArg("spaceId");
	const auto userId = req.GetArg("userId");

	if (spaceId.empty() || userId.empty()) {
		res["err"] = "Params should be set";
		req.SetResponseStatus(server::http::HttpStatus::kBadRequest);
		return res.ExtractValue();
	}

	try {
		if (!_s.DeleteUser(headerUserId, spaceId, userId)) {
			req.SetResponseStatus(server::http::HttpStatus::kNotFound);
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
		LOG_WARNING() << "Fail to delete user: " << e.what();
		res["err"] = "Fail to delete user";
		req.SetResponseStatus(server::http::HttpStatus::kInternalServerError);
	}

	return res.ExtractValue();
}

formats::json::Value UserManage::UpdateUser(
	const std::string headerUserId,
	const server::http::HttpRequest& req,
	const formats::json::Value& body) const
{
	formats::json::ValueBuilder res;

	if (!body.HasMember("spaceId") || !body.HasMember("userId")) {
		res["err"] = "Params should be set";
		req.SetResponseStatus(server::http::HttpStatus::kBadRequest);
		return res.ExtractValue();
	}

	const auto spaceId = body["spaceId"].As<std::string>();
	const auto userId = body["userId"].As<std::string>();

	bool isRoleMode = false;
	Role::Type role;

	if (body.HasMember("role")){
		const auto roleStr = body["role"].As<std::string>();

		if (roleStr.empty()) {
			res["err"] = "Param should be not empty";
			req.SetResponseStatus(server::http::HttpStatus::kBadRequest);
			return res.ExtractValue();
		}

		role = Role::FromString(roleStr);

		if (!_s.ValidateRole(role)) {
			res["err"] = "Wrong role";
			req.SetResponseStatus(server::http::HttpStatus::kBadRequest);
			return res.ExtractValue();
		}

		isRoleMode =  true;
	}

	bool isOwnerMode = false;
	bool isOwner;

	if (body.HasMember("isOwner")){
		isOwner = body["isOwner"].As<bool>();
		isOwnerMode = true;
	}

	if (!isOwnerMode && !isRoleMode) {
		req.SetResponseStatus(server::http::HttpStatus::kBadRequest);
		res["err"] = "Params should be set";
		return res.ExtractValue();
	}

	try {
		if (!_s.UpdateUser(isRoleMode, role, isOwnerMode, isOwner, spaceId, userId, headerUserId)) {
			req.SetResponseStatus(server::http::HttpStatus::kNotFound);
		}
	} catch(errors::BadRequest& e) {
		// todo - maybe another exception needed for NotFound status
		req.SetResponseStatus(server::http::HttpStatus::kBadRequest);
		res["err"] = e.what();
		return res.ExtractValue();
	} catch(errors::NotFound& e) {
		req.SetResponseStatus(server::http::HttpStatus::kNotFound);
		res["err"] = e.what();
		return res.ExtractValue();
	} catch(const std::exception& e) {
		LOG_WARNING() << "Fail to update user: " << e.what();
		res["err"] = "Fail to update user";
		req.SetResponseStatus(server::http::HttpStatus::kInternalServerError);
	}

	return res.ExtractValue();
}

} // namespace svetit::space::handlers