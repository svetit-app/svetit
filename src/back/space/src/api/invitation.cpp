#include "invitation.hpp"
#include "../model/invitation_serialize.hpp"
#include "../service/service.hpp"
#include "../model/role.hpp"
#include <shared/headers.hpp>
#include <shared/errors.hpp>
#include <shared/errors_catchit.hpp>
#include <shared/paging.hpp>
#include <shared/paging_serialize.hpp>
#include <shared/parse/request.hpp>
#include <shared/schemas.hpp>
#include <shared/parse/uuid.hpp>

namespace svetit::space::handlers {

Invitation::Invitation(
	const components::ComponentConfig& conf,
	const components::ComponentContext& ctx)
	: server::handlers::HttpHandlerJsonBase{conf, ctx}
	, _s{ctx.FindComponent<Service>()}
	, _mapHttpMethodToSchema{LoadSchemas(kName, _s.GetJSONSchemasPath())}
{}

formats::json::Value Invitation::HandleRequestJsonThrow(
	const server::http::HttpRequest& req,
	const formats::json::Value& body,
	server::request::RequestContext&) const
{
	formats::json::ValueBuilder res;

	try {
		const auto params = ValidateRequest(_mapHttpMethodToSchema, req, body);
		const auto userId = params[headers::kUserId].As<std::string>();

		switch (req.GetMethod()) {
		case server::http::HttpMethod::kGet:
			return GetList(res, userId, params);
		case server::http::HttpMethod::kPost:
			return Post(req, body, res, userId);
		case server::http::HttpMethod::kPut:
			return ChangeRole(req, body, res, userId, params);
		case server::http::HttpMethod::kPatch:
			return Join(req, res, userId, params);
		case server::http::HttpMethod::kDelete:
			return Delete(req, res, userId, params);
		default:
			throw std::runtime_error("Unsupported");
			break;
		}
	} catch(...) {
		return errors::CatchIt(req);
	}
	return res.ExtractValue();
}

formats::json::Value Invitation::GetList(
	formats::json::ValueBuilder& res,
	const std::string& userId,
	const formats::json::Value& params) const
{
	const auto paging = parsePaging(params);

	const auto spaceId = params["spaceId"].As<boost::uuids::uuid>(boost::uuids::uuid{});
	if (!spaceId.is_nil()) {
		res = _s.GetInvitationListBySpace(spaceId, paging.start, paging.limit, userId);
		return res.ExtractValue();
	}

	res = _s.GetInvitationList(paging.start, paging.limit, userId);

	return res.ExtractValue();
}

formats::json::Value Invitation::Post(
	const server::http::HttpRequest& req,
	const formats::json::Value& body,
	formats::json::ValueBuilder& res,
	const std::string& userId) const
{
	auto invitation = body.As<model::SpaceInvitation>();
	_s.Invite(userId, invitation.spaceId, invitation.userId, invitation.roleId);

	req.SetResponseStatus(server::http::HttpStatus::kCreated);
	return res.ExtractValue();
}

formats::json::Value Invitation::ChangeRole(
	const server::http::HttpRequest& req,
	const formats::json::Value& body,
	formats::json::ValueBuilder& res,
	const std::string& userId,
	const formats::json::Value& params) const
{
	const auto id = params["id"].As<int>();
	const auto roleId = body["roleId"].As<int>();

	_s.ChangeRoleInInvitation(id, roleId, userId);

	req.SetResponseStatus(server::http::HttpStatus::kNoContent);
	return res.ExtractValue();
}

formats::json::Value Invitation::Join(
	const server::http::HttpRequest& req,
	formats::json::ValueBuilder& res,
	const std::string& userId,
	const formats::json::Value& params) const
{
	const auto id = params["id"].As<int>();

	_s.ApproveInvitation(id, userId);

	req.SetResponseStatus(server::http::HttpStatus::kNoContent);
	return res.ExtractValue();
}

formats::json::Value Invitation::Delete(
	const server::http::HttpRequest& req,
	formats::json::ValueBuilder& res,
	const std::string& userId,
	const formats::json::Value& params) const
{
	const auto id = params["id"].As<int>();
	_s.DeleteInvitation(id, userId);

	req.SetResponseStatus(server::http::HttpStatus::kNoContent);
	return res.ExtractValue();
}

} // namespace svetit::space::handlers
