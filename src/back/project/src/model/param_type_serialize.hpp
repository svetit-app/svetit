#pragma once

#include "param_type.hpp"

#include <userver/formats/json/value.hpp>
#include <userver/formats/parse/common_containers.hpp>
#include <userver/formats/serialize/common_containers.hpp>
#include <userver/utest/using_namespace_userver.hpp>

namespace svetit::project::model {

formats::json::Value Serialize(
	const ParamType& item,
	formats::serialize::To<formats::json::Value>);

ParamType Parse(const formats::json::Value& json,
	formats::parse::To<ParamType>);

} // namespace svetit::project::model