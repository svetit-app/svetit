#pragma once

#include "translation.hpp"

#include <userver/formats/json/value.hpp>
#include <userver/formats/parse/common_containers.hpp>
#include <userver/formats/serialize/common_containers.hpp>
#include <userver/utest/using_namespace_userver.hpp>

namespace svetit::project::model {

formats::json::Value Serialize(
	const Translation& item,
	formats::serialize::To<formats::json::Value>);

Translation Parse(const formats::json::Value& json,
	formats::parse::To<Translation>);

} // namespace svetit::project::model