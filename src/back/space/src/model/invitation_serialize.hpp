#pragma once

#include "space_invitation.hpp"

#include <userver/formats/json/value.hpp>
#include <userver/formats/parse/common_containers.hpp>
#include <userver/formats/serialize/common_containers.hpp>
#include <userver/utest/using_namespace_userver.hpp>

namespace svetit::space::model {

formats::json::Value Serialize(
	const svetit::space::model::SpaceInvitation& si,
	formats::serialize::To<formats::json::Value>);

model::SpaceInvitation Parse(const formats::json::Value& json,
	formats::parse::To<model::SpaceInvitation>);

} // namespace svetit::space::model