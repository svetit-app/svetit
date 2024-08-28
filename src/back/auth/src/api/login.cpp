#include "login.hpp"
#include "helpers.hpp"
#include "../service/service.hpp"

#include <exception>
#include <string_view>
#include <userver/server/http/http_request.hpp>
#include <userver/server/http/http_status.hpp>
#include <userver/http/common_headers.hpp>
#include <userver/http/url.hpp>
#include <userver/http/parser/http_request_parse_args.hpp>

namespace svetit::auth::handlers {

Login::Login(
	const components::ComponentConfig& conf,
	const components::ComponentContext& ctx)
	: server::handlers::HttpHandlerBase{conf, ctx}
	, _s{ctx.FindComponent<Service>()}
	, _mapHttpMethodToSchema{LoadSchemas(kName, _s.GetJSONSchemasPath())}
{}

std::string Login::HandleRequestThrow(
	const server::http::HttpRequest& req,
	server::request::RequestContext&) const
{
	const auto params = ValidateRequest(_mapHttpMethodToSchema, req);

	auto redirectPath = params["redirectPath"]
		.As<std::string>(formats::json::Value::DefaultConstructed{});
	if (redirectPath.empty()) {
		redirectPath = params[http::headers::kReferer]
			.As<std::string>(formats::json::Value::DefaultConstructed{});

		if (!redirectPath.empty())
			redirectPath = http::ExtractPath(redirectPath);
	}

	auto callbackUrl = getCallerUrl(req, params, /*addApiPrefix*/true);
	callbackUrl += "/auth/login/callback";

	http::Args args;
	if (!redirectPath.empty())
		args.emplace("redirectPath", redirectPath);
	callbackUrl = http::MakeUrl(callbackUrl, args);

	std::string url;
	try {
		url = _s.GetLoginUrl(callbackUrl);
	}
	catch (const std::exception& e) {
		LOG_WARNING() << "GetLoginUrl:" << e.what();
		url = getCallerUrl(req, params);
		url = _s.GetErrorPageUrl(url);
	}

	auto& response = req.GetHttpResponse();
	response.SetStatus(server::http::HttpStatus::kFound);
	response.SetHeader(http::headers::kLocation, url);
	return {};
}

} // namespace svetit::auth::handlers
