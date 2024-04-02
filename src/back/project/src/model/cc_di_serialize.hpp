#pragma once

#include "cc_di.hpp"

#include <userver/formats/json/value.hpp>
#include <userver/formats/parse/common_containers.hpp>
#include <userver/formats/serialize/common_containers.hpp>
#include <userver/utest/using_namespace_userver.hpp>

namespace svetit::project::model {

formats::json::Value Serialize(
	const CcDi& ccDi,
	formats::serialize::To<formats::json::Value>);

CcDi Parse(const formats::json::Value& json,
	formats::parse::To<CcDi>);

} // namespace svetit::project::model