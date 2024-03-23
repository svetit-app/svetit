#include "user-manage.hpp"
#include "../model/user_serialize.hpp"
#include "../service/service.hpp"
#include <shared/headers.hpp>
#include <shared/errors.hpp>
#include <shared/errors_catchit.hpp>
#include <shared/paging.hpp>
#include <shared/parse/request.hpp>

#include <userver/components/component_config.hpp>
#include <userver/components/component_context.hpp>
#include <userver/server/handlers/http_handler_json_base.hpp>
#include <userver/utest/using_namespace_userver.hpp>
#include <userver/utils/boost_uuid4.hpp>

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
		const auto userId = req.GetHeader(headers::kUserId);
		if (userId.empty())
			throw errors::Unauthorized401();

		switch (req.GetMethod()) {
			case server::http::HttpMethod::kDelete:
				return Delete(userId, req, res);
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
	formats::json::ValueBuilder& res) const
{
	const auto spaceId = parseUUID(req, "spaceId");
	const auto userId = req.GetArg("userId");
	if (userId.empty())
		throw errors::BadRequest400{"Param usedrId should be set"};

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
