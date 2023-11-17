#include "space.hpp"
#include "../service/service.hpp"
#include "../../../shared/headers.hpp"
#include "../../../shared/errors.hpp"
#include "../model/space_serialize.hpp"

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
		case server::http::HttpMethod::kHead:
			return Head(req, body);
		default: break;
	}

	formats::json::ValueBuilder res;
	res["err"] = "Unsupported";
	req.SetResponseStatus(server::http::HttpStatus::kInternalServerError);
	return res.ExtractValue();
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

	try {
		switch (method) {
			case 1:
				res = _s.GetById(id, userId);
				break;
			case 2:
				res = _s.GetByKey(key, userId);
				break;
			case 3:
				res = _s.GetByLink(link);
				break;
		}
	} catch(errors::BadRequest& e) {
		req.SetResponseStatus(server::http::HttpStatus::kBadRequest);
		res["err"] = e.what();
		return res.ExtractValue();
	} catch(errors::NotFound& e) {
		req.SetResponseStatus(server::http::HttpStatus::kNotFound);
		res["err"] = e.what();
		return res.ExtractValue();
	} catch(const std::exception& e) {
		LOG_WARNING() << "Fail to get space: " << e.what();
		res["err"] = "Fail to get space";
		req.SetResponseStatus(server::http::HttpStatus::kInternalServerError);
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

	auto space = body.As<model::Space>();

	if (_s.isSpaceExistsByKey(space.key)) {
		req.SetResponseStatus(server::http::HttpStatus::kConflict);
		res["err"] = "Invalid key";
		return res.ExtractValue();
	}

	try {
		if (_s.Create(space.name, space.key, space.requestsAllowed, userId)) {
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

formats::json::Value Space::Head(
	const server::http::HttpRequest& req,
	const formats::json::Value& body) const
{
	formats::json::ValueBuilder res;

	const auto& key = req.GetArg("key");

	if (key.empty()) {
		req.SetResponseStatus(server::http::HttpStatus::kBadRequest);
		return res.ExtractValue();
	}

	try {
		if (!_s.isSpaceExistsByKey(key)) {
			req.SetResponseStatus(server::http::HttpStatus::kNotFound);
		}
	} catch(const std::exception& e) {
		LOG_WARNING() << "Fail to check space existence by key: " << e.what();
		req.SetResponseStatus(server::http::HttpStatus::kInternalServerError);
	}

	return res.ExtractValue();
}

} // namespace svetit::space::handlers