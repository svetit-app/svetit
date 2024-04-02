#pragma once

#include "plugin.hpp"

#include <userver/formats/json/value.hpp>
#include <userver/formats/parse/common_containers.hpp>
#include <userver/formats/serialize/common_containers.hpp>
#include <userver/utest/using_namespace_userver.hpp>

namespace svetit::project::model {

formats::json::Value Serialize(
	const Plugin& plugin,
	formats::serialize::To<formats::json::Value>);

Plugin Parse(const formats::json::Value& json,
	formats::parse::To<Plugin>);

} // namespace svetit::project::model