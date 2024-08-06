#pragma once

#include "group.hpp"

#include <userver/formats/json/value.hpp>
#include <userver/formats/parse/common_containers.hpp>
#include <userver/formats/serialize/common_containers.hpp>
#include <userver/utest/using_namespace_userver.hpp>

namespace svetit::space::model {

formats::json::Value Serialize(
	const Group& item,
	formats::serialize::To<formats::json::Value>);

Group Parse(const formats::json::Value& json,
	formats::parse::To<Group>);

} // namespace svetit::space::model