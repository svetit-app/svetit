#pragma once

#include "model.hpp"
#include "date/date.h"
#include <vector>

#include <userver/formats/json/value.hpp>
#include <userver/formats/parse/common_containers.hpp>
#include <userver/formats/serialize/common_containers.hpp>
#include <userver/utest/using_namespace_userver.hpp>

namespace svetit::space {

formats::json::Value Serialize(
	const std::vector<svetit::space::SpaceLink> sl,
	formats::serialize::To<formats::json::Value>);
} // namespace svetit::space
