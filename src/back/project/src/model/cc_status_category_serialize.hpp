#pragma once

#include "cc_status_category.hpp"

#include <userver/formats/json/value.hpp>
#include <userver/formats/parse/common_containers.hpp>
#include <userver/formats/serialize/common_containers.hpp>
#include <userver/utest/using_namespace_userver.hpp>

namespace svetit::project::model {

formats::json::Value Serialize(
	const CcStatusCategory& item,
	formats::serialize::To<formats::json::Value>);

CcStatusCategory Parse(const formats::json::Value& json,
	formats::parse::To<CcStatusCategory>);

} // namespace svetit::project::model