#pragma once

#include "cc_type_param.hpp"

#include <userver/formats/json/value.hpp>
#include <userver/formats/parse/common_containers.hpp>
#include <userver/formats/serialize/common_containers.hpp>
#include <userver/utest/using_namespace_userver.hpp>

namespace svetit::project::model {

formats::json::Value Serialize(
	const CcTypeParam& ccTypeParam,
	formats::serialize::To<formats::json::Value>);

CcTypeParam Parse(const formats::json::Value& json,
	formats::parse::To<CcTypeParam>);

} // namespace svetit::project::model