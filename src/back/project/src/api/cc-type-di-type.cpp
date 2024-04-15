#include "cc-type-di-type.hpp"
#include "../service/service.hpp"
#include "../model/cc_type_di_type.hpp"
#include "../model/cc_type_di_type_serialize.hpp"
#include <shared/errors.hpp>
#include <shared/errors_catchit.hpp>
#include <shared/parse/request.hpp>

namespace svetit::project::handlers {

CcTypeDiType::CcTypeDiType(
	const components::ComponentConfig& conf,
	const components::ComponentContext& ctx)
	: server::handlers::HttpHandlerJsonBase{conf, ctx}
	, _s{ctx.FindComponent<Service>()}
{}

formats::json::Value CcTypeDiType::HandleRequestJsonThrow(
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

formats::json::Value CcTypeDiType::Get(
	const server::http::HttpRequest& req,
	formats::json::ValueBuilder& res) const
{
	const auto ccTypeId = parsePositiveInt(req, "ccTypeId");
	const auto diTypeId = parsePositiveInt(req, "diTypeId");
	res = _s.GetCcTypeDiType(ccTypeId, diTypeId);

	return res.ExtractValue();
}

formats::json::Value CcTypeDiType::Post(
	const server::http::HttpRequest& req,
	const formats::json::Value& body,
	formats::json::ValueBuilder& res) const
{
	const auto ccTypeDiType = body.As<model::CcTypeDiType>();

	_s.CreateCcTypeDiType(ccTypeDiType);

	req.SetResponseStatus(server::http::HttpStatus::kCreated);
	return res.ExtractValue();
}

formats::json::Value CcTypeDiType::Delete(
	const server::http::HttpRequest& req,
	formats::json::ValueBuilder& res) const
{
	const auto ccTypeId = parsePositiveInt(req, "ccTypeId");
	const auto diTypeId = parsePositiveInt(req, "diTypeId");
	_s.DeleteCcTypeDiType(ccTypeId, diTypeId);

	return res.ExtractValue();
}

} // namespace svetit::project::handlers
