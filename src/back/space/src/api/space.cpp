#include "space.hpp"
#include "../service/service.hpp"
#include "../../../shared/headers.hpp"

namespace svetit::space::handlers {

Space::Space(
	const components::ComponentConfig& conf,
	const components::ComponentContext& ctx)
	: server::handlers::HttpHandlerJsonBase{conf, ctx}
	, _s{ctx.FindComponent<Service>()}
{}

formats::json::Value Space::HandleRequestJsonThrow(
	const server::http::HttpRequest& req,
	const formats::json::Value& body,
	server::request::RequestContext&) const
{
	// todo - is default needed?
	switch (req.GetMethod()) {
		case server::http::HttpMethod::kGet:
			return Get(req, body);
		case server::http::HttpMethod::kPost:
			return Post(req, body);
		case server::http::HttpMethod::kDelete:
			return Delete(req, body);
	}
}

formats::json::Value Space::Get(
	const server::http::HttpRequest& req,
	const formats::json::Value& body) const
{
	formats::json::ValueBuilder res;

	const auto& userId = req.GetHeader(headers::kUserId);
	if (userId.empty()) {
		res["err"] = "Access denied";
		req.SetResponseStatus(server::http::HttpStatus::kUnauthorized);
		return res.ExtractValue();
	}

	const auto& id = req.GetArg("id");
	const auto& key = req.GetArg("key");
	const auto& link = req.GetArg("link");

	if (id.empty() && key.empty() && link.empty()) {
		req.SetResponseStatus(server::http::HttpStatus::kBadRequest);
		res["err"] = "No arguments";
		return res.ExtractValue();
	}

	if (
		(!id.empty() && (!key.empty() || !link.empty()))
		|| (!key.empty() && (!id.empty() || !link.empty()))
		|| (!link.empty() && (!id.empty() || !key.empty()))
	) {
		req.SetResponseStatus(server::http::HttpStatus::kBadRequest);
		res["err"] = "Only one argument should be set";
		return res.ExtractValue();
	}

	int method;

	if (!id.empty()) {
		method = 1;
	} else if (!key.empty()) {
		// todo - rewtite this, because key with UUID (that equals userId) inside could exists, it will not pass standard key regex
		if (!_s.CheckKeyByRegex(key)) {
			req.SetResponseStatus(server::http::HttpStatus::kBadRequest);
			res["err"] = "Key must be valid";
			return res.ExtractValue();
		}
		method = 2;
	} else if (!link.empty()) {
		method = 3;
	}

	model::Space space = {};
	bool found = false;

	try {
		switch (method) {
			case 1:
				space = _s.GetById(id, found, userId);
				break;
			case 2:
				space = _s.GetByKey(key, found, userId);
				break;
			case 3:
				space = _s.GetByLink(link, found);
				break;
		}
	} catch(const std::exception& e) {
		LOG_WARNING() << "Fail to get space: " << e.what();
		res["err"] = "Fail to get space";
		req.SetResponseStatus(server::http::HttpStatus::kInternalServerError);
	}

	if (found) {
		res = space;
	} else {
		req.SetResponseStatus(server::http::HttpStatus::kNotFound);
	}

	return res.ExtractValue();
}

formats::json::Value Space::Delete(
	const server::http::HttpRequest& req,
	const formats::json::Value& body) const
{
	formats::json::ValueBuilder res;

	const auto& userId = req.GetHeader(headers::kUserId);
	if (userId.empty()) {
		res["err"] = "Access denied";
		req.SetResponseStatus(server::http::HttpStatus::kUnauthorized);
		return res.ExtractValue();
	}

	const auto& id = req.GetArg("id");

	if (id.empty()) {
		req.SetResponseStatus(server::http::HttpStatus::kBadRequest);
		res["err"] = "Param id must be set";
		return res.ExtractValue();
	}

	try {
		if (!_s.Delete(id, userId))
			req.SetResponseStatus(server::http::HttpStatus::kNotFound);
	}
	catch(const std::exception& e) {
		LOG_WARNING() << "Fail to delete space: " << e.what();
		res["err"] = "Fail to delete space";
		req.SetResponseStatus(server::http::HttpStatus::kInternalServerError);
	}

	return res.ExtractValue();
}

formats::json::Value Space::Post(
	const server::http::HttpRequest& req,
	const formats::json::Value& body) const
{
	formats::json::ValueBuilder res;

	const auto& userId = req.GetHeader(headers::kUserId);
	if (userId.empty()) {
		res["err"] = "Access denied";
		req.SetResponseStatus(server::http::HttpStatus::kUnauthorized);
		return res.ExtractValue();
	}

	if (!body.HasMember("key")) {
		req.SetResponseStatus(server::http::HttpStatus::kBadRequest);
		res["err"] = "No key param in body";
		return res.ExtractValue();
	}

	if (!body.HasMember("name")) {
		req.SetResponseStatus(server::http::HttpStatus::kBadRequest);
		res["err"] = "No name param in body";
		return res.ExtractValue();
	}

	if (!body.HasMember("requestsAllowed")) {
		req.SetResponseStatus(server::http::HttpStatus::kBadRequest);
		res["err"] = "No requestsAllowed param in body";
		return res.ExtractValue();
	}

	auto name = body["name"].ConvertTo<std::string>();
	auto key = body["key"].ConvertTo<std::string>();
	auto requestsAllowed = body["requestsAllowed"].ConvertTo<bool>();

	boost::trim(name);
	boost::trim(key);

	if (_s.isSpaceExistsByKey(key)) {
		req.SetResponseStatus(server::http::HttpStatus::kConflict);
		res["err"] = "Invalid key";
		return res.ExtractValue();
	}

	try {
		if (_s.Create(name, key, requestsAllowed, userId)) {
			req.SetResponseStatus(server::http::HttpStatus::kCreated);
		} else {
			req.SetResponseStatus(server::http::HttpStatus::kBadRequest);
			res["err"] = "Can't create space";
		}
	}
	catch(const std::exception& e) {
		LOG_WARNING() << "Fail to create space: " << e.what();
		res["err"] = "Fail to create space";
		req.SetResponseStatus(server::http::HttpStatus::kInternalServerError);
	}

	return res.ExtractValue();
}

} // namespace svetit::space::handlers