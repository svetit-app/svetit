#pragma once

#include "oidctokens.hpp"

#include <userver/formats/json/value.hpp>
#include <userver/formats/parse/common_containers.hpp>
#include <userver/formats/serialize/common_containers.hpp>
#include <userver/utest/using_namespace_userver.hpp>

namespace svetit::auth {

OIDCTokens Parse(const formats::json::Value& json, formats::parse::To<OIDCTokens>);

} // namespace svetit::auth
