#include "user-manage.hpp"
#include "../service/service.hpp"
#include "../../../shared/headers.hpp"

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
		res["err"] = "Empty userId header";
		req.SetResponseStatus(server::http::HttpStatus::kUnauthorized);
		return res.ExtractValue();
	}

	switch (req.GetMethod()) {
		case server::http::HttpMethod::kDelete:
			return Delete(userId, req, body);
		case server::http::HttpMethod::kPatch:
			return UpdateUser(userId, req, body);
	}

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
		res["err"] = "Params spaceId and userId should be set";
		req.SetResponseStatus(server::http::HttpStatus::kBadRequest);
		return res.ExtractValue();
	}

	if (!_s.ValidateUUID(spaceId) || !_s.ValidateUUID(userId)) {
		res["err"] = "Params spaceId and userId should be valid UUIDs";
		req.SetResponseStatus(server::http::HttpStatus::kBadRequest);
		return res.ExtractValue();
	}

	try {
		if (!_s.DeleteUser(headerUserId, spaceId, userId)) {
			req.SetResponseStatus(server::http::HttpStatus::kNotFound);
		}
	}
	catch(const std::exception& e) {
		LOG_WARNING() << "Fail to delete user: " << e.what();
		res["err"] = "Fail to delete user";
		req.SetResponseStatus(server::http::HttpStatus::kInternalServerError);
	}

	return res.ExtractValue();
}

formats::json::Value UserManage::UpdateUser(
	const std::string userId,
	const server::http::HttpRequest& req,
	const formats::json::Value& body) const
{
	formats::json::ValueBuilder res;

	return res.ExtractValue();
}

} // namespace svetit::space::handlers