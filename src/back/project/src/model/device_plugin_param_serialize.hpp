#pragma once

#include "device_plugin_param.hpp"

#include <userver/formats/json/value.hpp>
#include <userver/formats/parse/common_containers.hpp>
#include <userver/formats/serialize/common_containers.hpp>
#include <userver/utest/using_namespace_userver.hpp>

namespace svetit::project::model {

formats::json::Value Serialize(
	const DevicePluginParam& devicePluginParam,
	formats::serialize::To<formats::json::Value>);

DevicePluginParam Parse(const formats::json::Value& json,
	formats::parse::To<DevicePluginParam>);

} // namespace svetit::project::model