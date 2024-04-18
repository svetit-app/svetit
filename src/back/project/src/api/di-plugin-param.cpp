#include "di-plugin-param.hpp"
#include "../service/service.hpp"
#include "../model/di_plugin_param.hpp"
#include "../model/di_plugin_param_serialize.hpp"
#include <shared/errors.hpp>
#include <shared/errors_catchit.hpp>
#include <shared/parse/request.hpp>

namespace svetit::project::handlers {

DiPluginParam::DiPluginParam(
	const components::ComponentConfig& conf,
	const components::ComponentContext& ctx)
	: server::handlers::HttpHandlerJsonBase{conf, ctx}
	, _s{ctx.FindComponent<Service>()}
{}

formats::json::Value DiPluginParam::HandleRequestJsonThrow(
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

formats::json::Value DiPluginParam::Get(
	const server::http::HttpRequest& req,
	formats::json::ValueBuilder& res) const
{
	const auto diTypeId = parsePositiveInt(req, "diTypeId");
	const auto paramId = parsePositiveInt(req, "paramId");
	res = _s.GetDiPluginParam(diTypeId, paramId);

	return res.ExtractValue();
}

formats::json::Value DiPluginParam::Post(
	const server::http::HttpRequest& req,
	const formats::json::Value& body,
	formats::json::ValueBuilder& res) const
{
	const auto diPluginParam = body.As<model::DiPluginParam>();

	_s.CreateDiPluginParam(diPluginParam);

	req.SetResponseStatus(server::http::HttpStatus::kCreated);
	return res.ExtractValue();
}

formats::json::Value DiPluginParam::Delete(
	const server::http::HttpRequest& req,
	formats::json::ValueBuilder& res) const
{
	const auto diTypeId = parsePositiveInt(req, "diTypeId");
	const auto paramId = parsePositiveInt(req, "paramId");
	_s.DeleteDiPluginParam(diTypeId, paramId);

	return res.ExtractValue();
}

} // namespace svetit::project::handlers
