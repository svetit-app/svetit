#include "user_list.hpp"
#include "../model/userinfo_serialize.hpp"
#include "../service/service.hpp"
#include <svetit/headers.hpp>
#include <svetit/errors.hpp>
#include <svetit/errors_catchit.hpp>
#include <svetit/paging.hpp>

#include <userver/http/common_headers.hpp>

namespace svetit::auth::handlers {

UserList::UserList(
	const components::ComponentConfig& conf,
	const components::ComponentContext& ctx)
	: server::handlers::HttpHandlerJsonBase{conf, ctx}
	, _s{ctx.FindComponent<Service>()}
	, _mapHttpMethodToSchema{LoadSchemas(kName, _s.GetJSONSchemasPath())}
{}

formats::json::Value UserList::HandleRequestJsonThrow(
	const server::http::HttpRequest& req,
	const formats::json::Value& body,
	server::request::RequestContext&) const
{
	formats::json::ValueBuilder res;

	try {
		const auto params = ValidateRequest(_mapHttpMethodToSchema, req, body);
		const auto sessionId = params[headers::kSessionId].As<std::string>();
		const auto paging = parsePaging(params);
		const auto search = params["search"].As<std::string>("");

		auto items = _s.GetUserInfoList(search, sessionId, paging.start, paging.limit);
		
		res = std::map<std::string, std::vector<model::UserInfo>>{
			{"list", std::move(items)}
		};
	} catch(...) {
		return errors::CatchIt(req);
	}

	return res.ExtractValue();
}

} // namespace svetit::auth::handlers
