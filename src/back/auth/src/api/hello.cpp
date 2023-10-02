#include "hello.hpp"

#include <fmt/format.h>


namespace svetit::auth::handlers {

std::string Hello::HandleRequestThrow(
	const server::http::HttpRequest &request,
	server::request::RequestContext &) const
{
	return SayHelloTo(request.GetArg("name"));
}

std::string SayHelloTo(std::string_view name) {
	if (name.empty())
		name = "unknown user";
	return fmt::format("Hello, {}!\n", name);
}

} // namespace svetit::auth
