#include "errors_catchit.hpp"

#include <userver/server/handlers/http_handler_json_base.hpp>
#include <userver/utest/using_namespace_userver.hpp>

namespace svetit::errors {

formats::json::Value CatchIt(const server::http::HttpRequest& req)
{
	static constexpr const char* errKey = "err";
	formats::json::ValueBuilder res;

	auto eptr = std::current_exception();
	try {
		if (eptr)
			std::rethrow_exception(eptr);
	} catch(const Exception& e) {
		req.SetResponseStatus(e.GetHttpStatus());
		res[errKey] = e.what();
	} catch(const std::exception& e) {
		LOG_ERROR()
			<< "Method: " << req.GetMethodStr()
			<< " Url: " << req.GetUrl()
			<< " Err: " << e.what()
			<< " Body: " << req.RequestBody().substr(0, 1024);
		res[errKey] = "Internal error";
		req.SetResponseStatus(server::http::HttpStatus::kInternalServerError);
	} catch (...) {
		throw;
	}
	return res.ExtractValue();
}

} // namespace svetit::errors