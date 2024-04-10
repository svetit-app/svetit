#include "request.hpp"

#include "../errors.hpp"

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

bool parseBool(const server::http::HttpRequest& req, const std::string& key)
{
	try {
		// is it right way of string to bool transformation?
		const bool value = (strcasecmp("true",req.GetArg(key).c_str()) == 0);
		return value;
	} catch(const std::exception& e) {
		const auto msg = fmt::format("Param {} parse err: {}", key, e.what());
		throw errors::BadRequest400(msg);
	}
	return 0;
}

} // namespace svetit