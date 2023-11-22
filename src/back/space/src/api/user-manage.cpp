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

	try {
		const auto& userId = req.GetHeader(headers::kUserId);
		if (userId.empty()) {
			res["err"] = "Access denied";
			req.SetResponseStatus(server::http::HttpStatus::kUnauthorized);
			return res.ExtractValue();
		}

		switch (req.GetMethod()) {
			case server::http::HttpMethod::kDelete:
				return Delete(userId, req, body, res);
			case server::http::HttpMethod::kPatch:
				return UpdateUser(userId, req, body, res);
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
	} catch(const std::exception& e) {
		LOG_WARNING() << "Fail to process user manage handle with method: "
			<< req.GetMethodStr() << " err: " << e.what();
		res["err"] = "Fail to process user manage";
		req.SetResponseStatus(server::http::HttpStatus::kInternalServerError);
	}

	return res.ExtractValue();
}

formats::json::Value UserManage::Delete(
	const std::string headerUserId,
	const server::http::HttpRequest& req,
	const formats::json::Value& body,
	formats::json::ValueBuilder& res) const
{
	const auto spaceId = req.GetArg("spaceId");
	const auto userId = req.GetArg("userId");

	if (spaceId.empty() || userId.empty())
		throw errors::BadRequest{"Params should be set"};

	if (!_s.DeleteUser(headerUserId, spaceId, userId))
		throw errors::NotFound{};

	return res.ExtractValue();
}

formats::json::Value UserManage::UpdateUser(
	const std::string headerUserId,
	const server::http::HttpRequest& req,
	const formats::json::Value& body,
	formats::json::ValueBuilder& res) const
{
	model::SpaceUser user = body.As<model::SpaceUser>();

	bool isRoleMode = !user.isOwner;
	if (isRoleMode && !_s.ValidateRole(user.role))
		throw errors::BadRequest{"Wrong role"};

	if (!_s.UpdateUser(isRoleMode, user.role, user.isOwner, user.spaceId, user.userId, headerUserId))
		throw errors::NotFound{};

	return res.ExtractValue();
}

} // namespace svetit::space::handlers