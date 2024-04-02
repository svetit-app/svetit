#pragma once

#include "save_timer.hpp"

#include <userver/formats/json/value.hpp>
#include <userver/formats/parse/common_containers.hpp>
#include <userver/formats/serialize/common_containers.hpp>
#include <userver/utest/using_namespace_userver.hpp>

namespace svetit::project::model {

formats::json::Value Serialize(
	const SaveTimer& saveTimer,
	formats::serialize::To<formats::json::Value>);

SaveTimer Parse(const formats::json::Value& json,
	formats::parse::To<SaveTimer>);

} // namespace svetit::project::model