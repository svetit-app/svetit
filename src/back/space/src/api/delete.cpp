#include "delete.hpp"

namespace svetit::space::handlers {

Delete::Delete(
	const components::ComponentConfig& conf,
	const components::ComponentContext& ctx)
	: server::handlers::HttpHandlerJsonBase{conf, ctx}
{}

formats::json::Value Delete::HandleRequestJsonThrow(
	const server::http::HttpRequest& req,
	const formats::json::Value& body,
	server::request::RequestContext&) const
{
	formats::json::ValueBuilder res;

	res = formats::json::FromString(R"({
    "items":[]
  	})");

	return res.ExtractValue();
}

} // namespace svetit::space::handlers
