#include "link.hpp"
#include "../service/service.hpp"
#include "../../../shared/headers.hpp"
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

	const auto& start = req.GetArg("start");
	const auto& limit = req.GetArg("limit");
	int iStart;
	int iLimit;

	if (!start.empty() && !limit.empty()){
		try {
			iStart = boost::lexical_cast<int>(start);
		} catch(const std::exception& e) {
			LOG_WARNING() << "Wrong start query param: " << e.what() << " , start=" << start;;
			req.SetResponseStatus(server::http::HttpStatus::kBadRequest);
			res["err"] = "Wrong start query param";
			return res.ExtractValue();
		}

		try {
			iLimit = boost::lexical_cast<int>(limit);
		} catch(const std::exception& e) {
			LOG_WARNING() << "Wrong limit query param: " << e.what() << " , limit=" << limit;
			req.SetResponseStatus(server::http::HttpStatus::kBadRequest);
			res["err"] = "Wrong limit query param";
			return res.ExtractValue();
		}

		if (iStart < 0) {
			LOG_WARNING() << "Start param must be unsigned int, gotten=" << iStart;
			req.SetResponseStatus(server::http::HttpStatus::kBadRequest);
			res["err"] = "Start param must be unsigned int";
			return res.ExtractValue();
		}

		if (iLimit <= 0) {
			LOG_WARNING() << "Limit param must be more then 0, gotten=" << iLimit;
			req.SetResponseStatus(server::http::HttpStatus::kBadRequest);
			res["err"] = "Limit param must be more then 0";
			return res.ExtractValue();
		}
	} else {
		LOG_WARNING() << "Start and Limit params must be set, gotten start=" << start << " limit=" << limit;
		req.SetResponseStatus(server::http::HttpStatus::kBadRequest);
		res["err"] = "Start and Limit params must be set";
		return res.ExtractValue();
	}

	try {
		res["list"] = _s.GetLinkList(iStart,iLimit);
		res["total"] = _s.GetLinksCount();
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

	// todo - need to validate that this is valid uuid? need to check that user exists?
	const auto& creatorId = req.GetHeader(headers::kUserId);
	if (creatorId.empty()) {
		res["err"] = "Empty userId header";
		req.SetResponseStatus(server::http::HttpStatus::kUnauthorized);
		return res.ExtractValue();
	}

	if (!body.HasMember("spaceId")) {
		LOG_WARNING() << "No spaceId param in body";
		req.SetResponseStatus(server::http::HttpStatus::kBadRequest);
		res["err"] = "No spaceId param in body";
		return res.ExtractValue();
	}

	if (!body.HasMember("name")) {
		LOG_WARNING() << "No name param in body";
		req.SetResponseStatus(server::http::HttpStatus::kBadRequest);
		res["err"] = "No name param in body";
		return res.ExtractValue();
	}

	if (!body.HasMember("expiredAt")) {
		LOG_WARNING() << "No expiredAt param in body";
		req.SetResponseStatus(server::http::HttpStatus::kBadRequest);
		res["err"] = "No expiredAt param in body";
		return res.ExtractValue();
	}

	const auto spaceId = body["spaceId"].ConvertTo<std::string>();
	const auto name = body["name"].ConvertTo<std::string>();
	const auto expiredAtStr = body["expiredAt"].ConvertTo<std::string>();

	std::chrono::system_clock::time_point expiredAt;

	std::tm tm = {};
	std::stringstream ss(expiredAtStr);
	ss >> std::get_time(&tm, "%Y-%m-%d %H:%M:%S");
	if (ss.fail()) {
		LOG_WARNING() << "Wrong expiredAt (parsing)";
		req.SetResponseStatus(server::http::HttpStatus::kBadRequest);
		res["err"] = "Wrong expiredAt (parsing)";
		return res.ExtractValue();
	}
	expiredAt = std::chrono::system_clock::from_time_t(std::mktime(&tm));

	if (!_s.CheckExpiredAtValidity(expiredAt)) {
		LOG_WARNING() << "Wrong expiredAt (less than now)";
		req.SetResponseStatus(server::http::HttpStatus::kBadRequest);
		res["err"] = "Wrong expiredAt (less than now)";
		return res.ExtractValue();
	}

	if (spaceId.empty() || name.empty()) {
		LOG_WARNING() << "SpaceId, name, expiredAt params must be set";
		req.SetResponseStatus(server::http::HttpStatus::kBadRequest);
		res["err"] = "SpaceId, name, expiredAt params must be set";
		return res.ExtractValue();
	}

	if (!_s.ValidateUUID(spaceId)){
		LOG_WARNING() << "SpaceId must be valid uuid";
		req.SetResponseStatus(server::http::HttpStatus::kBadRequest);
		res["err"] = "SpaceId must be valid uuid";
		return res.ExtractValue();
	}

	if (!_s.CheckLinkNameByRegex(name)){
		LOG_WARNING() << "Name must be valid by pattern";
		req.SetResponseStatus(server::http::HttpStatus::kBadRequest);
		res["err"] = "Name must be valid by pattern";
		return res.ExtractValue();
	}

	try {
		if (!_s.CreateInvitationLink(spaceId, creatorId, name, expiredAt)){
			req.SetResponseStatus(server::http::HttpStatus::kBadRequest);
		}
	}
	catch(const std::exception& e) {
		LOG_WARNING() << "Fail to create invitation: " << e.what();
		res["err"] = "Fail to create invitation";
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
	}
}

} // namespace svetit::space::handlers
