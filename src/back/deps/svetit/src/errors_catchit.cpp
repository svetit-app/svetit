#include <svetit/errors_catchit.hpp>
#include <svetit/headers.hpp>

#include <userver/server/handlers/http_handler_json_base.hpp>
#include <userver/utest/using_namespace_userver.hpp>

namespace svetit::errors {

formats::json::Value CatchIt(const server::http::HttpRequest& req)
{
	static constexpr const char* errKey = "err";
	formats::json::ValueBuilder res;

	std::string text;

	auto eptr = std::current_exception();
	try {
		if (eptr)
			std::rethrow_exception(eptr);
	} catch(const Exception& e) {
		if (e.NeedsLogging())
			text = e.what();

		req.SetResponseStatus(e.GetHttpStatus());
		res[errKey] = e.what();
	} catch(const std::exception& e) {
		text = e.what();
		req.SetResponseStatus(server::http::HttpStatus::kInternalServerError);
		res[errKey] = "Internal error";
	} catch (...) {
		throw;
	}

	if (!text.empty())
	{
		std::string userId;
		if (req.HasHeader(headers::kUserId))
			userId = req.GetHeader(headers::kUserId);
		LOG_ERROR()
			<< "Method: " << req.GetMethodStr()
			<< " Url: " << req.GetUrl()
			<< " User: " << userId
			<< " Err: " << text
			<< " Body: " << req.RequestBody().substr(0, 1024);
	}

	return res.ExtractValue();
}

} // namespace svetit::errors
