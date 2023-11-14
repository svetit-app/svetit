#pragma once

#include "space.hpp"

#include <userver/formats/json/value.hpp>
#include <userver/formats/parse/common_containers.hpp>
#include <userver/formats/serialize/common_containers.hpp>
#include <userver/utest/using_namespace_userver.hpp>

namespace svetit::space::model {

formats::json::Value Serialize(
	const svetit::space::model::Space& s,
	formats::serialize::To<formats::json::Value>);

Space Parse(const formats::json::Value& json,
	formats::parse::To<Space>);

} // namespace svetit::space::model