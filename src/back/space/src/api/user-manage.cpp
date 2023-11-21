#include "user-manage.hpp"
#include "../service/service.hpp"
#include "../../../shared/headers.hpp"
#include "../../../shared/errors.hpp"
#include "../model/user_serialize.hpp"

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

	try {
		const auto spaceId = req.GetArg("spaceId");
		const auto userId = req.GetArg("userId");

		if (spaceId.empty() || userId.empty())
			throw errors::BadRequest{"Params should be set"};

		if (!_s.DeleteUser(headerUserId, spaceId, userId))
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

	try {

		model::SpaceUser user = body.As<model::SpaceUser>();

		bool isRoleMode = !user.isOwner;
		if (isRoleMode && !_s.ValidateRole(user.role))
			throw errors::BadRequest{"Wrong role"};

		if (!_s.UpdateUser(isRoleMode, user.role, user.isOwner, user.spaceId, user.userId, headerUserId))
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
		LOG_WARNING() << "Fail to update user: " << e.what();
		res["err"] = "Fail to update user";
		req.SetResponseStatus(server::http::HttpStatus::kInternalServerError);
	}

	return res.ExtractValue();
}

} // namespace svetit::space::handlers