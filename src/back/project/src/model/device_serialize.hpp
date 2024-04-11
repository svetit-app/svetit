#pragma once

#include "device.hpp"

#include <userver/formats/json/value.hpp>
#include <userver/formats/parse/common_containers.hpp>
#include <userver/formats/serialize/common_containers.hpp>
#include <userver/utest/using_namespace_userver.hpp>

namespace svetit::project::model {

formats::json::Value Serialize(
	const Device& item,
	formats::serialize::To<formats::json::Value>);

Device Parse(const formats::json::Value& json,
	formats::parse::To<Device>);

} // namespace svetit::project::model