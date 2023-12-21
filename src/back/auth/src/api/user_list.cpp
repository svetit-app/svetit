#include "user_list.hpp"
#include "../model/userinfo_serialize.hpp"
#include "../service/service.hpp"
#include "../../../shared/headers.hpp"

#include <stdexcept>

#include <userver/http/common_headers.hpp>

namespace svetit::auth::handlers {

UserList::UserList(
	const components::ComponentConfig& conf,
	const components::ComponentContext& ctx)
	: server::handlers::HttpHandlerJsonBase{conf, ctx}
	, _s{ctx.FindComponent<Service>()}
{}

formats::json::Value UserList::HandleRequestJsonThrow(
	const server::http::HttpRequest& req,
	const formats::json::Value& body,
	server::request::RequestContext&) const
{
	formats::json::ValueBuilder res;

	return res.ExtractValue();
}

} // namespace svetit::auth::handlers
