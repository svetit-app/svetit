#include "cc-status-category.hpp"
#include "../service/service.hpp"
#include "../model/cc_status_category.hpp"
#include "../model/cc_status_category_serialize.hpp"
#include <shared/errors.hpp>
#include <shared/errors_catchit.hpp>
#include <shared/parse/request.hpp>

namespace svetit::project::handlers {

CcStatusCategory::CcStatusCategory(
	const components::ComponentConfig& conf,
	const components::ComponentContext& ctx)
	: server::handlers::HttpHandlerJsonBase{conf, ctx}
	, _s{ctx.FindComponent<Service>()}
{}

formats::json::Value CcStatusCategory::HandleRequestJsonThrow(
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
		case server::http::HttpMethod::kPatch:
			return Patch(req, body, res);
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

formats::json::Value CcStatusCategory::Get(
	const server::http::HttpRequest& req,
	formats::json::ValueBuilder& res) const
{
	const auto id = parsePositiveInt(req, "id");
	res = _s.GetCcStatusCategory(id);

	return res.ExtractValue();
}

formats::json::Value CcStatusCategory::Post(
	const server::http::HttpRequest& req,
	const formats::json::Value& body,
	formats::json::ValueBuilder& res) const
{
	const auto ccStatusCategory = body.As<model::CcStatusCategory>();

	_s.CreateCcStatusCategory(ccStatusCategory);

	req.SetResponseStatus(server::http::HttpStatus::kCreated);
	return res.ExtractValue();
}

formats::json::Value CcStatusCategory::Patch(
	const server::http::HttpRequest& req,
	const formats::json::Value& body,
	formats::json::ValueBuilder& res) const
{
	const auto ccStatusCategory = body.As<model::CcStatusCategory>();

	_s.UpdateCcStatusCategory(ccStatusCategory);

	return res.ExtractValue();
}

formats::json::Value CcStatusCategory::Delete(
	const server::http::HttpRequest& req,
	formats::json::ValueBuilder& res) const
{
	const auto id = parsePositiveInt(req, "id");

	_s.DeleteCcStatusCategory(id);

	return res.ExtractValue();
}

} // namespace svetit::project::handlers
