#include "cc-type-param.hpp"
#include "../service/service.hpp"
#include "../model/cc_type_param.hpp"
#include "../model/cc_type_param_serialize.hpp"
#include <shared/errors.hpp>
#include <shared/errors_catchit.hpp>
#include <shared/parse/request.hpp>

namespace svetit::project::handlers {

CcTypeParam::CcTypeParam(
	const components::ComponentConfig& conf,
	const components::ComponentContext& ctx)
	: server::handlers::HttpHandlerJsonBase{conf, ctx}
	, _s{ctx.FindComponent<Service>()}
{}

formats::json::Value CcTypeParam::HandleRequestJsonThrow(
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

formats::json::Value CcTypeParam::Get(
	const server::http::HttpRequest& req,
	formats::json::ValueBuilder& res) const
{
	const auto ccTypeId = parsePositiveInt(req, "ccTypeId");
	const auto paramId = parsePositiveInt(req, "paramId");
	auto spaceId = utils::BoostUuidFromString(req.GetHeader("X-Space-Id"));
	res = _s.GetCcTypeParam(spaceId, ccTypeId, paramId);

	return res.ExtractValue();
}

formats::json::Value CcTypeParam::Post(
	const server::http::HttpRequest& req,
	const formats::json::Value& body,
	formats::json::ValueBuilder& res) const
{
	auto ccTypeParam = body.As<model::CcTypeParam>();
	auto spaceId = utils::BoostUuidFromString(req.GetHeader("X-Space-Id"));
	ccTypeParam.spaceId = spaceId;

	_s.CreateCcTypeParam(ccTypeParam);

	req.SetResponseStatus(server::http::HttpStatus::kCreated);
	return res.ExtractValue();
}

formats::json::Value CcTypeParam::Delete(
	const server::http::HttpRequest& req,
	formats::json::ValueBuilder& res) const
{
	const auto ccTypeId = parsePositiveInt(req, "ccTypeId");
	const auto paramId = parsePositiveInt(req, "paramId");
	auto spaceId = utils::BoostUuidFromString(req.GetHeader("X-Space-Id"));
	_s.DeleteCcTypeParam(spaceId, ccTypeId, paramId);

	return res.ExtractValue();
}

} // namespace svetit::project::handlers
