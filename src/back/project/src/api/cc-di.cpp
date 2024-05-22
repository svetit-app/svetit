#include "cc-di.hpp"
#include "../service/service.hpp"
#include "../model/cc_di.hpp"
#include "../model/cc_di_serialize.hpp"
#include <shared/errors.hpp>
#include <shared/errors_catchit.hpp>
#include <shared/parse/request.hpp>

namespace svetit::project::handlers {

CcDi::CcDi(
	const components::ComponentConfig& conf,
	const components::ComponentContext& ctx)
	: server::handlers::HttpHandlerJsonBase{conf, ctx}
	, _s{ctx.FindComponent<Service>()}
{}

formats::json::Value CcDi::HandleRequestJsonThrow(
	const server::http::HttpRequest& req,
	const formats::json::Value& body,
	server::request::RequestContext&) const
{
	formats::json::ValueBuilder res;

	try {
		switch (req.GetMethod()) {
		case server::http::HttpMethod::kGet:
			return Get(req, res);
		case server::http::HttpMethod::kPost:
			return Post(req, body, res);
		case server::http::HttpMethod::kDelete:
			return Delete(req, res);
		default:
			throw std::runtime_error("Unsupported");
			break;
		}
	} catch(...) {
		return errors::CatchIt(req);
	}

	return res.ExtractValue();
}

formats::json::Value CcDi::Get(
	const server::http::HttpRequest& req,
	formats::json::ValueBuilder& res) const
{
	const auto ccId = parsePositiveInt(req, "ccId");
	const auto diId = parsePositiveInt(req, "diId");
	const auto spaceId = utils::BoostUuidFromString(req.GetHeader("X-Space-Id"));
	res = _s.GetCcDi(spaceId, ccId, diId);

	return res.ExtractValue();
}

formats::json::Value CcDi::Post(
	const server::http::HttpRequest& req,
	const formats::json::Value& body,
	formats::json::ValueBuilder& res) const
{
	auto ccDi = body.As<model::CcDi>();
	auto spaceId = utils::BoostUuidFromString(req.GetHeader("X-Space-Id"));
	ccDi.spaceId = spaceId;

	_s.CreateCcDi(spaceId, ccDi);

	req.SetResponseStatus(server::http::HttpStatus::kCreated);
	return res.ExtractValue();
}

formats::json::Value CcDi::Delete(
	const server::http::HttpRequest& req,
	formats::json::ValueBuilder& res) const
{
	const auto ccId = parsePositiveInt(req, "ccId");
	const auto diId = parsePositiveInt(req, "diId");
	auto spaceId = utils::BoostUuidFromString(req.GetHeader("X-Space-Id"));
	_s.DeleteCcDi(spaceId, ccId, diId);

	return res.ExtractValue();
}

} // namespace svetit::project::handlers
