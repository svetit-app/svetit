#pragma once

#include "session_refresh.hpp"

#include <userver/formats/json/value.hpp>
#include <userver/formats/serialize/common_containers.hpp>
#include <userver/utest/using_namespace_userver.hpp>

namespace svetit::auth::model {

formats::json::Value Serialize(
	const model::SessionRefresh& s,
	formats::serialize::To<formats::json::Value>);

} // namespace svetit::auth::model