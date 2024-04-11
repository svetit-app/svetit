#pragma once

#include "section_param.hpp"

#include <userver/formats/json/value.hpp>
#include <userver/formats/parse/common_containers.hpp>
#include <userver/formats/serialize/common_containers.hpp>
#include <userver/utest/using_namespace_userver.hpp>

namespace svetit::project::model {

formats::json::Value Serialize(
	const SectionParam& item,
	formats::serialize::To<formats::json::Value>);

SectionParam Parse(const formats::json::Value& json,
	formats::parse::To<SectionParam>);

} // namespace svetit::project::model