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
#include <shared/type_utils.hpp>

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
			return GetList(req, res, userId, params);
		case server::http::HttpMethod::kPost:
			return Post(req, body, res, userId, params);
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
	const server::http::HttpRequest& req,
	formats::json::ValueBuilder& res,
	const std::string& userId,
	const formats::json::Value& params) const
{
	Paging paging = {
		.start = params["start"].As<int>(),
		.limit = params["limit"].As<int>()
	};

	if (_s.IsListLimit(paging.limit))
		throw errors::BadRequest400("Too big limit param");

	if (params.HasMember("spaceId")) {
		const auto spaceId = params["spaceId"].As<boost::uuids::uuid>();
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
	const std::string& userId,
	const formats::json::Value& params) const
{
	if (params.HasMember("link")) {
		const auto link = params["link"].As<boost::uuids::uuid>();

		if (!_s.InviteByLink(userId, link))
			throw errors::BadRequest400{"Link expired"};

		req.SetResponseStatus(server::http::HttpStatus::kCreated);
		return res.ExtractValue();
	}

	auto invitation = body.As<model::SpaceInvitation>();

	if (invitation.spaceId.is_nil() || invitation.userId.empty())
		throw errors::BadRequest400{"Params must be set"};

	_s.Invite(userId, invitation.spaceId, invitation.userId, invitation.role);

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

	if (!body.HasMember("role"))
		throw errors::BadRequest400{"No role param in body"};

	const auto role = Role::FromString(body["role"].As<std::string>());

	_s.ChangeRoleInInvitation(id, role, userId);

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

	return res.ExtractValue();
}

} // namespace svetit::space::handlers
