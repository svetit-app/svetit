#pragma once

#include "role.hpp"
#include "space_user.hpp"
#include <vector>

#include <boost/uuid/uuid_io.hpp>

#include <userver/formats/json/value.hpp>
#include <userver/formats/parse/common_containers.hpp>
#include <userver/formats/serialize/common_containers.hpp>
#include <userver/utest/using_namespace_userver.hpp>

namespace svetit::space::model {

formats::json::Value Serialize(
	const svetit::space::model::SpaceUser& su,
	formats::serialize::To<formats::json::Value>);

model::SpaceUser Parse(const formats::json::Value& json,
	formats::parse::To<model::SpaceUser>);

} // namespace svetit::space::model