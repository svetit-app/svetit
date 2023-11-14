#include "link.hpp"
#include "../service/service.hpp"
#include "../../../shared/headers.hpp"
#include "../../../shared/errors.hpp"
#include <boost/date_time.hpp>

namespace svetit::space::handlers {

Link::Link(
	const components::ComponentConfig& conf,
	const components::ComponentContext& ctx)
	: server::handlers::HttpHandlerJsonBase{conf, ctx}
	, _s{ctx.FindComponent<Service>()}
{}

formats::json::Value Link::GetList(
	const server::http::HttpRequest& req,
	const formats::json::Value& body) const
{
	formats::json::ValueBuilder res;

	try {
		auto paging = paging::parsePaging(req);
		res["list"] = _s.GetLinkList(paging.start, paging.limit);
		res["total"] = _s.GetLinksCount();
	}
	catch(const errors::BadRequest& e) {
		res["err"] = e.what();
		req.SetResponseStatus(server::http::HttpStatus::kBadRequest);
		return res.ExtractValue();
	}
	catch(const std::exception& e) {
		LOG_WARNING() << " Fail to get invitation links list: " << e.what();
		req.SetResponseStatus(server::http::HttpStatus::kInternalServerError);
	}

	return res.ExtractValue();
}

formats::json::Value Link::Post(
	const server::http::HttpRequest& req,
	const formats::json::Value& body) const
{
	formats::json::ValueBuilder res;

	const auto& creatorId = req.GetHeader(headers::kUserId);
	if (creatorId.empty()) {
		res["err"] = "Access denied";
		req.SetResponseStatus(server::http::HttpStatus::kUnauthorized);
		return res.ExtractValue();
	}

	if (!body.HasMember("spaceId")) {
		req.SetResponseStatus(server::http::HttpStatus::kBadRequest);
		res["err"] = "No spaceId param";
		return res.ExtractValue();
	}

	if (!body.HasMember("name")) {
		req.SetResponseStatus(server::http::HttpStatus::kBadRequest);
		res["err"] = "No name param";
		return res.ExtractValue();
	}

	if (!body.HasMember("expiredAt")) {
		req.SetResponseStatus(server::http::HttpStatus::kBadRequest);
		res["err"] = "No expiredAt param";
		return res.ExtractValue();
	}

	const auto spaceId = body["spaceId"].ConvertTo<std::string>();
	const auto name = body["name"].ConvertTo<std::string>();
	// todo - is need to make such conversion inside try/catch with lexical_cast?
	const auto expiredAt = body["expiredAt"].ConvertTo<int64_t>();

	if (!_s.CheckExpiredAtValidity(expiredAt)) {
		req.SetResponseStatus(server::http::HttpStatus::kBadRequest);
		res["err"] = "Wrong expiredAt";
		return res.ExtractValue();
	}

	if (spaceId.empty() || name.empty()) {
		req.SetResponseStatus(server::http::HttpStatus::kBadRequest);
		res["err"] = "Params must be set";
		return res.ExtractValue();
	}

	try {
		_s.CreateInvitationLink(spaceId, creatorId, name, expiredAt);
	}
	catch(const std::exception& e) {
		LOG_WARNING() << "Fail to create invitation: " << e.what();
		res["err"] = "Fail to create invitation";
		req.SetResponseStatus(server::http::HttpStatus::kInternalServerError);
	}

	return res.ExtractValue();
}

formats::json::Value Link::Delete(
	const server::http::HttpRequest& req,
	const formats::json::Value& body) const
{
	formats::json::ValueBuilder res;

	const auto& id = req.GetArg("id");

	if (id.empty()) {
		req.SetResponseStatus(server::http::HttpStatus::kBadRequest);
		res["err"] = "Param id must be set";
		return res.ExtractValue();
	}

	try {
		if (!_s.DeleteInvitationLink(id)) {
			req.SetResponseStatus(server::http::HttpStatus::kNotFound);
		}
	}
	catch(const std::exception& e) {
		LOG_WARNING() << "Fail to delete invitation link: " << e.what();
		res["err"] = "Fail to delete invitation link";
		req.SetResponseStatus(server::http::HttpStatus::kInternalServerError);
	}

	return res.ExtractValue();
}

formats::json::Value Link::HandleRequestJsonThrow(
	const server::http::HttpRequest& req,
	const formats::json::Value& body,
	server::request::RequestContext&) const
{
	// todo - is default needed?
	switch (req.GetMethod()) {
		case server::http::HttpMethod::kGet:
			return GetList(req, body);
		case server::http::HttpMethod::kPost:
			return Post(req, body);
		case server::http::HttpMethod::kDelete:
			return Delete(req, body);
	}
}

} // namespace svetit::space::handlers