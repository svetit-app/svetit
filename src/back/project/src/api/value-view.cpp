#include "value-view.hpp"
#include "../service/service.hpp"
#include "../model/value_view.hpp"
#include "../model/value_view_serialize.hpp"
#include <shared/errors.hpp>
#include <shared/errors_catchit.hpp>
#include <shared/parse/request.hpp>

namespace svetit::project::handlers {

ValueView::ValueView(
	const components::ComponentConfig& conf,
	const components::ComponentContext& ctx)
	: server::handlers::HttpHandlerJsonBase{conf, ctx}
	, _s{ctx.FindComponent<Service>()}
{}

formats::json::Value ValueView::HandleRequestJsonThrow(
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

formats::json::Value ValueView::Get(
	const server::http::HttpRequest& req,
	formats::json::ValueBuilder& res) const
{
	const auto id = parsePositiveInt(req, "id");
	res = _s.GetValueView(id);

	return res.ExtractValue();
}

formats::json::Value ValueView::Post(
	const server::http::HttpRequest& req,
	const formats::json::Value& body,
	formats::json::ValueBuilder& res) const
{
	const auto valueView = body.As<model::ValueView>();

	_s.CreateValueView(valueView);

	req.SetResponseStatus(server::http::HttpStatus::kCreated);
	return res.ExtractValue();
}

formats::json::Value ValueView::Patch(
	const server::http::HttpRequest& req,
	const formats::json::Value& body,
	formats::json::ValueBuilder& res) const
{
	const auto valueView = body.As<model::ValueView>();

	_s.UpdateValueView(valueView);

	return res.ExtractValue();
}

formats::json::Value ValueView::Delete(
	const server::http::HttpRequest& req,
	formats::json::ValueBuilder& res) const
{
	const auto id = parsePositiveInt(req, "id");

	_s.DeleteValueView(id);

	return res.ExtractValue();
}
} // namespace svetit::project::handlers
