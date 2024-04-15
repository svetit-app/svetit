#include "section-param.hpp"
#include "../service/service.hpp"
#include "../model/section_param.hpp"
#include "../model/section_param_serialize.hpp"
#include <shared/errors.hpp>
#include <shared/errors_catchit.hpp>
#include <shared/parse/request.hpp>

namespace svetit::project::handlers {

SectionParam::SectionParam(
	const components::ComponentConfig& conf,
	const components::ComponentContext& ctx)
	: server::handlers::HttpHandlerJsonBase{conf, ctx}
	, _s{ctx.FindComponent<Service>()}
{}

formats::json::Value SectionParam::HandleRequestJsonThrow(
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

formats::json::Value SectionParam::Get(
	const server::http::HttpRequest& req,
	formats::json::ValueBuilder& res) const
{
	const auto sectionId = parsePositiveInt(req, "sectionId");
	const auto paramId = parsePositiveInt(req, "paramId");
	res = _s.GetSectionParam(sectionId, paramId);

	return res.ExtractValue();
}

formats::json::Value SectionParam::Post(
	const server::http::HttpRequest& req,
	const formats::json::Value& body,
	formats::json::ValueBuilder& res) const
{
	const auto sectionParam = body.As<model::SectionParam>();

	_s.CreateSectionParam(sectionParam);

	req.SetResponseStatus(server::http::HttpStatus::kCreated);
	return res.ExtractValue();
}

formats::json::Value SectionParam::Delete(
	const server::http::HttpRequest& req,
	formats::json::ValueBuilder& res) const
{
	const auto sectionId = parsePositiveInt(req, "sectionId");
	const auto paramId = parsePositiveInt(req, "paramId");
	_s.DeleteSectionParam(sectionId, paramId);

	return res.ExtractValue();
}

} // namespace svetit::project::handlers
