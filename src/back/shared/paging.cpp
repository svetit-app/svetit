#include "paging.hpp"

#include <userver/components/component_config.hpp>
#include <userver/components/component_context.hpp>
#include <userver/server/handlers/http_handler_json_base.hpp>
#include <userver/utest/using_namespace_userver.hpp>
#include <userver/utils/boost_uuid4.hpp>

namespace svetit {

Paging parsePaging(const server::http::HttpRequest& req)
{
	try {
		Paging info{
			.start = std::stoi(req.GetArg("start")),
			.limit = std::stoi(req.GetArg("limit"))
		};

		if (info.start < 0 || info.limit < 0)
			throw errors::BadRequest("range params less then zero");
		return info;
	} catch(const std::exception& e) {
		throw errors::BadRequest(e.what());
	}
	return {};
}

} // svetit