#pragma once

#include "cc_mode_type.hpp"

#include <userver/formats/json/value.hpp>
#include <userver/formats/parse/common_containers.hpp>
#include <userver/formats/serialize/common_containers.hpp>
#include <userver/utest/using_namespace_userver.hpp>

namespace svetit::project::model {

formats::json::Value Serialize(
	const CcModeType& ccModeType,
	formats::serialize::To<formats::json::Value>);

CcModeType Parse(const formats::json::Value& json,
	formats::parse::To<CcModeType>);

} // namespace svetit::project::model