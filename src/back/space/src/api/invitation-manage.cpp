#include "invitation-manage.hpp"
#include "../service/service.hpp"

namespace svetit::space::handlers {

InvitationManage::InvitationManage(
	const components::ComponentConfig& conf,
	const components::ComponentContext& ctx)
	: server::handlers::HttpHandlerJsonBase{conf, ctx}
	, _s{ctx.FindComponent<Service>()}
{}

formats::json::Value InvitationManage::HandleRequestJsonThrow(
	const server::http::HttpRequest& req,
	const formats::json::Value& body,
	server::request::RequestContext&) const
{
	formats::json::ValueBuilder res;

	const auto& id = req.GetPathArg("id");

	if (id.empty()) {
		LOG_WARNING() << "Path param id must be set";
		req.SetResponseStatus(server::http::HttpStatus::kBadRequest);
		res["err"] = "Path param id must be set";
		return res.ExtractValue();
	}

	int iId;

	try {
		iId = boost::lexical_cast<int>(id);
	} catch(const std::exception& e) {
		LOG_WARNING() << "Id path param must be int: " << e.what() << " , id=" << id;
		req.SetResponseStatus(server::http::HttpStatus::kBadRequest);
		res["err"] = "Id path param must be int";
		return res.ExtractValue();
	}

	if (iId < 0) {
		LOG_WARNING() << "Id path param must be more than zero, id=" << iId;
		req.SetResponseStatus(server::http::HttpStatus::kBadRequest);
		res["err"] = "Id path param must be more than zero";
		return res.ExtractValue();
	}

	switch (req.GetMethod()) {
		case server::http::HttpMethod::kPut:
			return ChangeRole(iId, req, body);
		case server::http::HttpMethod::kPatch:
			return Join(iId, req, body);
		case server::http::HttpMethod::kDelete:
			return Delete(iId, req, body);
	}
}

formats::json::Value InvitationManage::ChangeRole(
	const int id,
	const server::http::HttpRequest& req,
	const formats::json::Value& body) const
{
	formats::json::ValueBuilder res;

	if (!body.HasMember("role")) {
		LOG_WARNING() << "No role param in body";
		req.SetResponseStatus(server::http::HttpStatus::kBadRequest);
		res["err"] = "No role param in body";
		return res.ExtractValue();
	}

	const auto role = body["role"].ConvertTo<std::string>();

	if (!_s.ValidateRole(role)) {
		LOG_WARNING() << "Wrong role";
		req.SetResponseStatus(server::http::HttpStatus::kBadRequest);
		res["err"] = "Wrong role";
		return res.ExtractValue();
	}

	try {
		if (!_s.ChangeRoleInInvitation(id, role)){
			req.SetResponseStatus(server::http::HttpStatus::kNotModified);
		}
	}
	catch(const std::exception& e) {
		LOG_WARNING() << "Fail to change role in invitation: " << e.what();
		res["err"] = "Fail to change role in invitation";
		req.SetResponseStatus(server::http::HttpStatus::kInternalServerError);
	}

	return res.ExtractValue();
}

formats::json::Value InvitationManage::Join(
	const int id,
	const server::http::HttpRequest& req,
	const formats::json::Value& body) const
{
	formats::json::ValueBuilder res;

	try {
		if (!_s.ApproveInvitation(id)) {
			req.SetResponseStatus(server::http::HttpStatus::kNotModified);
		}
	}
	catch(const std::exception& e) {
		LOG_WARNING() << "Fail to approve invitation: " << e.what();
		res["err"] = "Fail to approve invitation";
		req.SetResponseStatus(server::http::HttpStatus::kInternalServerError);
	}

	return res.ExtractValue();
}

formats::json::Value InvitationManage::Delete(
	const int id,
	const server::http::HttpRequest& req,
	const formats::json::Value& body) const
{
	formats::json::ValueBuilder res;

	try {
		if (!_s.DeleteInvitation(id)) {
			req.SetResponseStatus(server::http::HttpStatus::kNotModified);
		}
	}
	catch(const std::exception& e) {
		LOG_WARNING() << "Fail to delete invitation: " << e.what();
		res["err"] = "Fail to delete invitation";
		req.SetResponseStatus(server::http::HttpStatus::kInternalServerError);
	}

	return res.ExtractValue();
}

} // namespace svetit::space::handlers