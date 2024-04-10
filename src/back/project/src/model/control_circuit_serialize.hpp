#pragma once

#include "control_circuit.hpp"

#include <userver/formats/json/value.hpp>
#include <userver/formats/parse/common_containers.hpp>
#include <userver/formats/serialize/common_containers.hpp>
#include <userver/utest/using_namespace_userver.hpp>

namespace svetit::project::model {

formats::json::Value Serialize(
	const ControlCircuit& item,
	formats::serialize::To<formats::json::Value>);

ControlCircuit Parse(const formats::json::Value& json,
	formats::parse::To<ControlCircuit>);

} // namespace svetit::project::model