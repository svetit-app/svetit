#pragma once

#include "role.hpp"

#include <userver/formats/json/value.hpp>
#include <userver/formats/parse/common_containers.hpp>
#include <userver/formats/serialize/common_containers.hpp>
#include <userver/utest/using_namespace_userver.hpp>

namespace svetit::space::model {

formats::json::Value Serialize(
	const Role& item,
	formats::serialize::To<formats::json::Value>);

Role Parse(const formats::json::Value& json,
	formats::parse::To<Role>);

} // namespace svetit::space::model