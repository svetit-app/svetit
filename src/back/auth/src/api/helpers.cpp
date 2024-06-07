#include "helpers.hpp"

namespace svetit::auth::handlers {

std::string getCallerUrl(
	const server::http::HttpRequest& req,
	const formats::json::Value& params,
	bool addApiPrefix)
{
	std::string scheme;
	if (!params.HasMember("X-Forwarded-Proto")) {
		scheme = "http";
	} else {
		scheme = params["X-Forwarded-Proto"].As<std::string>();
	}

	std::string host;
	if (!params.HasMember("X-Forwarded-Host")) {
		host = req.GetHost();
	} else {
		host = params["X-Forwarded-Host"].As<std::string>();
	}

	std::string apiPrefix;
	if (addApiPrefix) {
		if (params.HasMember("X-ApiPrefix")) {
			apiPrefix = params["X-ApiPrefix"].As<std::string>();
			if (apiPrefix != "" && apiPrefix[0] != '/')
				apiPrefix = "/" + apiPrefix;
		}
	}

	return scheme + "://" + host + apiPrefix;
}

} // namespace svetit::auth::handlers
