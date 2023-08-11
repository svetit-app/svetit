#include "helpers.hpp"

namespace svetit::auth::handlers {

std::string getCallerUrl(const server::http::HttpRequest& req, bool addApiPrefix)
{
	auto scheme = req.GetHeader("X-Forwarded-Proto");
	if (scheme.empty())
		scheme = "http";

	auto host = req.GetHeader("X-Forwarded-Host");
	if (host.empty())
		host = req.GetHost();

	std::string apiPrefix;
	if (addApiPrefix)
	{
		apiPrefix = req.GetHeader("X-ApiPrefix");
		if (apiPrefix != "" && apiPrefix[0] != '/')
			apiPrefix = "/" + apiPrefix;
	}

	return scheme + "://" + host + apiPrefix;
}

} // namespace svetit::auth::handlers
