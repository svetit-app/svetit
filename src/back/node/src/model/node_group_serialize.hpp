#pragma once

#include "node_group.hpp"

#include <userver/formats/json/value.hpp>
#include <userver/formats/parse/common_containers.hpp>
#include <userver/formats/serialize/common_containers.hpp>
#include <userver/utest/using_namespace_userver.hpp>

namespace svetit::node::model {

formats::json::Value Serialize(
	const NodeGroup& item,
	formats::serialize::To<formats::json::Value>);

NodeGroup Parse(const formats::json::Value& json,
	formats::parse::To<NodeGroup>);

} // namespace svetit::node::model