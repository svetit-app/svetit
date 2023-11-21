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

	try {
		const auto& userId = req.GetHeader(headers::kUserId);
		if (userId.empty())
			throw errors::Unauthorized{};

		const auto& id = req.GetArg("id");
		const auto& key = req.GetArg("key");
		const auto& link = req.GetArg("link");

		if (id.empty() && key.empty() && link.empty())
			throw errors::BadRequest{"No arguments"};

		if (
			(!id.empty() && (!key.empty() || !link.empty()))
			|| (!key.empty() && (!id.empty() || !link.empty()))
			|| (!link.empty() && (!id.empty() || !key.empty()))
		)
			throw errors::BadRequest{"Only one argument should be set"};

		int method;

		if (!id.empty()) {
			method = 1;
		} else if (!key.empty()) {
			// todo - rewtite this, because key with UUID (that equals userId) inside could exists, it will not pass standard key regex
			if (!_s.CheckKeyByRegex(key))
				throw errors::BadRequest{"Key must be valid"};
			method = 2;
		} else if (!link.empty()) {
			method = 3;
		}

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
	} catch(const errors::Unauthorized& e) {
		req.SetResponseStatus(server::http::HttpStatus::kUnauthorized);
		res["err"] = e.what();
		return res.ExtractValue();
	} catch(const errors::BadRequest& e) {
		req.SetResponseStatus(server::http::HttpStatus::kBadRequest);
		res["err"] = e.what();
		return res.ExtractValue();
	} catch(const errors::NotFound& e) {
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

	try {
		const auto& userId = req.GetHeader(headers::kUserId);
		if (userId.empty())
			throw errors::Unauthorized{};

		const auto& id = req.GetArg("id");

		if (id.empty())
			throw errors::BadRequest{"Param id must be set"};

		if (!_s.Delete(id, userId))
			throw errors::NotFound{};
	} catch(const errors::Unauthorized& e) {
		req.SetResponseStatus(server::http::HttpStatus::kUnauthorized);
		res["err"] = e.what();
		return res.ExtractValue();
	} catch(const errors::BadRequest& e) {
		req.SetResponseStatus(server::http::HttpStatus::kBadRequest);
		res["err"] = e.what();
		return res.ExtractValue();
	} catch(const errors::NotFound& e) {
		req.SetResponseStatus(server::http::HttpStatus::kNotFound);
		res["err"] = e.what();
		return res.ExtractValue();
	} catch(const std::exception& e) {
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

	try {
		const auto& userId = req.GetHeader(headers::kUserId);
		if (userId.empty())
			throw errors::Unauthorized{};

		auto space = body.As<model::Space>();

		if (_s.isSpaceExistsByKey(space.key))
			throw errors::Conflict{"Invalid key"};

		if (!_s.Create(space.name, space.key, space.requestsAllowed, userId))
			throw errors::BadRequest{"Can't create space"};

		req.SetResponseStatus(server::http::HttpStatus::kCreated);
	} catch(const errors::Unauthorized& e) {
		req.SetResponseStatus(server::http::HttpStatus::kUnauthorized);
		res["err"] = e.what();
		return res.ExtractValue();
	} catch(const errors::BadRequest& e) {
		req.SetResponseStatus(server::http::HttpStatus::kBadRequest);
		res["err"] = e.what();
		return res.ExtractValue();
	} catch(const errors::Conflict& e) {
		req.SetResponseStatus(server::http::HttpStatus::kConflict);
		res["err"] = e.what();
		return res.ExtractValue();
	} catch(const std::exception& e) {
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

	try {
		const auto& key = req.GetArg("key");

		if (key.empty())
			throw errors::BadRequest{"Key param must be set"};

		if (!_s.isSpaceExistsByKey(key))
			throw errors::NotFound{};
	} catch(const errors::BadRequest& e) {
		req.SetResponseStatus(server::http::HttpStatus::kBadRequest);
		res["err"] = e.what();
		return res.ExtractValue();
	} catch(const errors::NotFound& e) {
		req.SetResponseStatus(server::http::HttpStatus::kNotFound);
		res["err"] = e.what();
		return res.ExtractValue();
	} catch(const std::exception& e) {
		LOG_WARNING() << "Fail to check space existence by key: " << e.what();
		req.SetResponseStatus(server::http::HttpStatus::kInternalServerError);
	}

	return res.ExtractValue();
}

} // namespace svetit::space::handlers