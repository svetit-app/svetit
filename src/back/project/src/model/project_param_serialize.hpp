#pragma once

#include "project_param.hpp"

#include <userver/formats/json/value.hpp>
#include <userver/formats/parse/common_containers.hpp>
#include <userver/formats/serialize/common_containers.hpp>
#include <userver/utest/using_namespace_userver.hpp>

namespace svetit::project::model {

formats::json::Value Serialize(
	const ProjectParam& projectParam,
	formats::serialize::To<formats::json::Value>);

ProjectParam Parse(const formats::json::Value& json,
	formats::parse::To<ProjectParam>);

} // namespace svetit::project::model