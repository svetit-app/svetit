#include "group-user.hpp"
#include "../model/group_user_serialize.hpp"
#include "../service/service.hpp"
#include <shared/headers.hpp>
#include <shared/errors.hpp>
#include <shared/errors_catchit.hpp>
#include <shared/paging.hpp>
#include <shared/parse/request.hpp>
#include <shared/schemas.hpp>

namespace svetit::node::handlers {

GroupUser::GroupUser(
	const components::ComponentConfig& conf,
	const components::ComponentContext& ctx)
	: server::handlers::HttpHandlerJsonBase{conf, ctx}
	, _s{ctx.FindComponent<Service>()}
	, _mapHttpMethodToSchema{LoadSchemas(kName, _s.GetJSONSchemasPath())}
{}

formats::json::Value GroupUser::HandleRequestJsonThrow(
	const server::http::HttpRequest& req,
	const formats::json::Value& body,
	server::request::RequestContext&) const
{
	formats::json::ValueBuilder res;

	try {
		const auto params = ValidateRequest(_mapHttpMethodToSchema, req, body);

		switch (req.GetMethod()) {
			case server::http::HttpMethod::kGet:
				return Get(res, params);
			case server::http::HttpMethod::kPost:
				return Post(req, body, res, params);
			case server::http::HttpMethod::kDelete:
				return Delete(req, res, params);
			default:
				throw std::runtime_error("Unsupported");
				break;
		}
	} catch(...) {
		return errors::CatchIt(req);
	}

	return res.ExtractValue();
}

formats::json::Value GroupUser::Get(
	formats::json::ValueBuilder& res,
	const formats::json::Value& params) const
{
	const auto headerUserId = params[headers::kUserId].As<std::string>();
	const auto groupId = params["groupId"].As<int>();
	const auto userId = params["userId"].As<std::string>();
	res = _s.GetGroupUser(groupId, userId, headerUserId);
	return res.ExtractValue();
}

formats::json::Value GroupUser::Delete(
	const server::http::HttpRequest& req,
	formats::json::ValueBuilder& res,
	const formats::json::Value& params) const
{
	const auto headerUserId = params[headers::kUserId].As<std::string>();

	const auto groupId = params["groupId"].As<int>();
	const auto userId = params["userId"].As<std::string>();

	_s.DeleteGroupUser(groupId, userId, headerUserId);

	req.SetResponseStatus(server::http::HttpStatus::kNoContent);
	return res.ExtractValue();
}

formats::json::Value GroupUser::Post(
	const server::http::HttpRequest& req,
	const formats::json::Value& body,
	formats::json::ValueBuilder& res,
	const formats::json::Value& params) const
{
	const auto headerUserId = params[headers::kUserId].As<std::string>();

	auto groupUser = body.As<model::GroupUser>();

	_s.CreateGroupUser(groupUser, headerUserId);

	req.SetResponseStatus(server::http::HttpStatus::kCreated);
	return res.ExtractValue();
}

} // namespace svetit::node::handlers
