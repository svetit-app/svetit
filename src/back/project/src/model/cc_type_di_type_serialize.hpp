#pragma once

#include "cc_type_di_type.hpp"

#include <userver/formats/json/value.hpp>
#include <userver/formats/parse/common_containers.hpp>
#include <userver/formats/serialize/common_containers.hpp>
#include <userver/utest/using_namespace_userver.hpp>

namespace svetit::project::model {

formats::json::Value Serialize(
	const CcTypeDiType& item,
	formats::serialize::To<formats::json::Value>);

CcTypeDiType Parse(const formats::json::Value& json,
	formats::parse::To<CcTypeDiType>);

} // namespace svetit::project::model