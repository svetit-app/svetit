#pragma once

#include "space_link.hpp"
#include "date/date.h"

#include <userver/formats/json/value.hpp>
#include <userver/formats/parse/common_containers.hpp>
#include <userver/formats/serialize/common_containers.hpp>
#include <userver/utest/using_namespace_userver.hpp>

namespace svetit::space::model {

formats::json::Value Serialize(
	const svetit::space::model::SpaceLink sl,
	formats::serialize::To<formats::json::Value>);

} // namespace svetit::space::model