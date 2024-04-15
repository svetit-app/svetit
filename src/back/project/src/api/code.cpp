#include "code.hpp"
#include "../service/service.hpp"
#include "../model/code.hpp"
#include "../model/code_serialize.hpp"
#include <shared/errors.hpp>
#include <shared/errors_catchit.hpp>
#include <shared/parse/request.hpp>

namespace svetit::project::handlers {

Code::Code(
	const components::ComponentConfig& conf,
	const components::ComponentContext& ctx)
	: server::handlers::HttpHandlerJsonBase{conf, ctx}
	, _s{ctx.FindComponent<Service>()}
{}

formats::json::Value Code::HandleRequestJsonThrow(
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

formats::json::Value Code::Get(
	const server::http::HttpRequest& req,
	formats::json::ValueBuilder& res) const
{
	const auto id = parsePositiveInt(req, "id");
	res = _s.GetCode(id);

	return res.ExtractValue();
}

formats::json::Value Code::Post(
	const server::http::HttpRequest& req,
	const formats::json::Value& body,
	formats::json::ValueBuilder& res) const
{
	const auto code = body.As<model::Code>();

	_s.CreateCode(code);

	req.SetResponseStatus(server::http::HttpStatus::kCreated);
	return res.ExtractValue();
}

formats::json::Value Code::Patch(
	const server::http::HttpRequest& req,
	const formats::json::Value& body,
	formats::json::ValueBuilder& res) const
{
	const auto code = body.As<model::Code>();

	_s.UpdateCode(code);

	return res.ExtractValue();
}

formats::json::Value Code::Delete(
	const server::http::HttpRequest& req,
	formats::json::ValueBuilder& res) const
{
	const auto id = parsePositiveInt(req, "id");

	_s.DeleteCode(id);

	return res.ExtractValue();
}

} // namespace svetit::project::handlers
