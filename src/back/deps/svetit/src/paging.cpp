#include <svetit/paging.hpp>
#include <svetit/errors.hpp>

#include <userver/server/handlers/http_handler_json_base.hpp>
#include <userver/utest/using_namespace_userver.hpp>
#include <userver/utils/boost_uuid4.hpp>

namespace svetit {

// TODO: remove after schema validation was used in all endpoints
Paging parsePaging(const server::http::HttpRequest& req)
{
	try {
		Paging info{
			.start = std::stoi(req.GetArg("start")),
			.limit = std::stoi(req.GetArg("limit"))
		};

		if (info.start < 0 || info.limit < 0)
			throw errors::BadRequest400("range params less then zero");
		return info;
	} catch(const std::exception& e) {
		throw errors::BadRequest400(e.what());
	}
	return {};
}

Paging parsePaging(const formats::json::Value& params)
{
	return Paging{
		.start = params["start"].As<int>(),
		.limit = params["limit"].As<int>()
	};
}

} // svetit
