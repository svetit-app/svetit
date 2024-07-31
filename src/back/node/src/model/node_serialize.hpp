#pragma once

#include "node.hpp"

#include <userver/formats/json/value.hpp>
#include <userver/formats/parse/common_containers.hpp>
#include <userver/formats/serialize/common_containers.hpp>
#include <userver/utest/using_namespace_userver.hpp>

namespace svetit::node::model {

formats::json::Value Serialize(
	const Node& item,
	formats::serialize::To<formats::json::Value>);

Node Parse(const formats::json::Value& json,
	formats::parse::To<Node>);

} // namespace svetit::node::model