#include "link.hpp"
#include "../service/service.hpp"

namespace svetit::space::handlers {

Link::Link(
	const components::ComponentConfig& conf,
	const components::ComponentContext& ctx)
	: server::handlers::HttpHandlerJsonBase{conf, ctx}
	, _s{ctx.FindComponent<Service>()}
{}

formats::json::Value Link::HandleRequestJsonThrow(
	const server::http::HttpRequest& req,
	const formats::json::Value& body,
	server::request::RequestContext&) const
{
	formats::json::ValueBuilder res;

	try {
		res["items"] = _s.GetLinkList();
	}
	catch(const std::exception& e) {
		LOG_WARNING() << " Fail to get invitation links list: " << e.what();
		req.SetResponseStatus(server::http::HttpStatus::kInternalServerError);
	}

	return res.ExtractValue();
}

} // namespace svetit::space::handlers
