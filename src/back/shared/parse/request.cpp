#include "request.hpp"

#include "../errors.hpp"

#include <userver/components/component_config.hpp>
#include <userver/components/component_context.hpp>
#include <userver/server/handlers/http_handler_json_base.hpp>
#include <userver/utest/using_namespace_userver.hpp>
#include <userver/utils/boost_uuid4.hpp>

namespace svetit {

int parsePositiveInt(const server::http::HttpRequest& req, const std::string& key)
{
	try {
		const int value = std::stoi(req.GetArg(key));
		if (value < 0)
			throw std::runtime_error{"must be greater then -1"};
		return value;
	} catch(const std::exception& e) {
		const auto msg = fmt::format("Param {} parse err: {}", key, e.what());
		throw errors::BadRequest400(msg);
	}
	return 0;
}

boost::uuids::uuid parseUUID(const server::http::HttpRequest& req, const std::string& key)
{
	try {
		return utils::BoostUuidFromString(req.GetArg(key));
	} catch (const std::exception& e) {
		const auto msg = fmt::format("Param {} parse err: {}", key, e.what());
		throw errors::BadRequest400(msg);
	}
	return {};
}

} // namespace svetit