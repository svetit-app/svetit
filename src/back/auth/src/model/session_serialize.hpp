#pragma once

#include "session.hpp"

#include <userver/formats/json/value.hpp>
#include <userver/formats/parse/common_containers.hpp>
#include <userver/formats/serialize/common_containers.hpp>
#include <userver/utest/using_namespace_userver.hpp>

namespace svetit::auth::model {

formats::json::Value Serialize(
	const model::SessionRefresh& s,
	formats::serialize::To<formats::json::Value>);
model::SessionRefresh Parse(const formats::json::Value& json, formats::parse::To<SessionRefresh>);

model::SessionRefresh ParseSessionRefreshInfo(const formats::json::Value& json);

} // namespace svetit::auth::model