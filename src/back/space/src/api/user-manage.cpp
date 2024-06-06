#include "user-manage.hpp"
#include "../model/user_serialize.hpp"
#include "../service/service.hpp"
#include <shared/headers.hpp>
#include <shared/errors.hpp>
#include <shared/errors_catchit.hpp>
#include <shared/paging.hpp>
#include <shared/parse/request.hpp>
#include <shared/schemas.hpp>
#include <shared/parse/uuid.hpp>

namespace svetit::space::handlers {

UserManage::UserManage(
	const components::ComponentConfig& conf,
	const components::ComponentContext& ctx)
	: server::handlers::HttpHandlerJsonBase{conf, ctx}
	, _s{ctx.FindComponent<Service>()}
	, _mapHttpMethodToSchema{LoadSchemas(kName, _s.GetJSONSchemasPath())}
{}

formats::json::Value UserManage::HandleRequestJsonThrow(
	const server::http::HttpRequest& req,
	const formats::json::Value& body,
	server::request::RequestContext&) const
{
	formats::json::ValueBuilder res;

	try {
		const auto params = ValidateRequest(_mapHttpMethodToSchema, req, body);
		const auto userId = params[headers::kUserId].As<std::string>();

		switch (req.GetMethod()) {
			case server::http::HttpMethod::kDelete:
				return Delete(userId, req, res, params);
			case server::http::HttpMethod::kPatch:
				return UpdateUser(userId, body, res);
			default:
				throw std::runtime_error("Unsupported");
				break;
		}
	} catch(...) {
		return errors::CatchIt(req);
	}

	return res.ExtractValue();
}

formats::json::Value UserManage::Delete(
	const std::string headerUserId,
	const server::http::HttpRequest& req,
	formats::json::ValueBuilder& res,
	const formats::json::Value& params) const
{
	const auto spaceId = params["spaceId"].As<boost::uuids::uuid>();
	const auto userId = params["userId"].As<std::string>();
	if (userId.empty())
		throw errors::BadRequest400{"Param userId should be set"};

	_s.DeleteUser(spaceId, userId, headerUserId);

	return res.ExtractValue();
}

formats::json::Value UserManage::UpdateUser(
	const std::string headerUserId,
	const formats::json::Value& body,
	formats::json::ValueBuilder& res) const
{
	model::SpaceUser user = body.As<model::SpaceUser>();

	if (!_s.UpdateUser(user, headerUserId))
		throw errors::BadRequest400{"Can't update user"};

	return res.ExtractValue();
}

} // namespace svetit::space::handlers
