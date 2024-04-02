#pragma once

#include "measure.hpp"

#include <userver/formats/json/value.hpp>
#include <userver/formats/parse/common_containers.hpp>
#include <userver/formats/serialize/common_containers.hpp>
#include <userver/utest/using_namespace_userver.hpp>

namespace svetit::project::model {

formats::json::Value Serialize(
	const Measure& measure,
	formats::serialize::To<formats::json::Value>);

Measure Parse(const formats::json::Value& json,
	formats::parse::To<Measure>);

} // namespace svetit::project::model