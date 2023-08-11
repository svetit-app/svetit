#pragma once

#include <string>
#include <string_view>

#include <userver/components/component_list.hpp>
#include <userver/utest/using_namespace_userver.hpp>

namespace svetit::auth::handlers {

std::string SayHelloTo(std::string_view name);

void AppendHello(components::ComponentList &component_list);

} // namespace svetit::auth
