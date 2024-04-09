#pragma once

#include "cc_type.hpp"

#include <userver/formats/json/value.hpp>
#include <userver/formats/parse/common_containers.hpp>
#include <userver/formats/serialize/common_containers.hpp>
#include <userver/utest/using_namespace_userver.hpp>

namespace svetit::project::model {

formats::json::Value Serialize(
	const CcType& ccType,
	formats::serialize::To<formats::json::Value>);

CcType Parse(const formats::json::Value& json,
	formats::parse::To<CcType>);

} // namespace svetit::project::model