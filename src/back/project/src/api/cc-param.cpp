#include "cc-param.hpp"
#include "../service/service.hpp"
#include "../model/cc_param.hpp"
#include "../model/cc_param_serialize.hpp"
#include <shared/errors.hpp>
#include <shared/errors_catchit.hpp>
#include <shared/parse/request.hpp>

namespace svetit::project::handlers {

CcParam::CcParam(
	const components::ComponentConfig& conf,
	const components::ComponentContext& ctx)
	: server::handlers::HttpHandlerJsonBase{conf, ctx}
	, _s{ctx.FindComponent<Service>()}
{}

formats::json::Value CcParam::HandleRequestJsonThrow(
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

formats::json::Value CcParam::Get(
	const server::http::HttpRequest& req,
	formats::json::ValueBuilder& res) const
{
	const auto ccId = parsePositiveInt(req, "ccId");
	const auto paramId = parsePositiveInt(req, "paramId");
	auto spaceId = utils::BoostUuidFromString(req.GetHeader("X-Space-Id"));
	res = _s.GetCcParam(spaceId, ccId, paramId);

	return res.ExtractValue();
}

formats::json::Value CcParam::Post(
	const server::http::HttpRequest& req,
	const formats::json::Value& body,
	formats::json::ValueBuilder& res) const
{
	auto ccParam = body.As<model::CcParam>();
	auto spaceId = utils::BoostUuidFromString(req.GetHeader("X-Space-Id"));
	ccParam.spaceId = spaceId;

	_s.CreateCcParam(ccParam);

	req.SetResponseStatus(server::http::HttpStatus::kCreated);
	return res.ExtractValue();
}

formats::json::Value CcParam::Delete(
	const server::http::HttpRequest& req,
	formats::json::ValueBuilder& res) const
{
	const auto ccId = parsePositiveInt(req, "ccId");
	const auto paramId = parsePositiveInt(req, "paramId");
	auto spaceId = utils::BoostUuidFromString(req.GetHeader("X-Space-Id"));

	_s.DeleteCcParam(spaceId, ccId, paramId);

	return res.ExtractValue();
}

} // namespace svetit::project::handlers
