#pragma once

#include "node_project.hpp"

#include <userver/formats/json/value.hpp>
#include <userver/formats/parse/common_containers.hpp>
#include <userver/formats/serialize/common_containers.hpp>
#include <userver/utest/using_namespace_userver.hpp>

namespace svetit::node::model {

formats::json::Value Serialize(
	const NodeProject& item,
	formats::serialize::To<formats::json::Value>);

NodeProject Parse(const formats::json::Value& json,
	formats::parse::To<NodeProject>);

} // namespace svetit::node::model