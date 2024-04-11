#pragma once

#include "project.hpp"

#include <userver/formats/json/value.hpp>
#include <userver/formats/parse/common_containers.hpp>
#include <userver/formats/serialize/common_containers.hpp>
#include <userver/utest/using_namespace_userver.hpp>

namespace svetit::project::model {

formats::json::Value Serialize(
	const Project& item,
	formats::serialize::To<formats::json::Value>);

Project Parse(const formats::json::Value& json,
	formats::parse::To<Project>);

} // namespace svetit::project::model