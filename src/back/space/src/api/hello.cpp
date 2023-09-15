#include "hello.hpp"

#include <fmt/format.h>

#include <userver/server/handlers/http_handler_base.hpp>

namespace svetit::space::handlers {

namespace {

class Hello final : public server::handlers::HttpHandlerBase {
public:
  static constexpr std::string_view kName = "handler-hello";

  using HttpHandlerBase::HttpHandlerBase;

  std::string HandleRequestThrow(
      const server::http::HttpRequest &request,
      server::request::RequestContext &) const override {
    return svetit::space::handlers::SayHelloTo(request.GetArg("name"));
  }
};

} // namespace

std::string SayHelloTo(std::string_view name) {
  if (name.empty()) {
    name = "unknown user";
  }

  return fmt::format("Hello, {}!\n", name);
}

void AppendHello(components::ComponentList &component_list) {
  component_list.Append<Hello>();
}

} // namespace svetit::space
