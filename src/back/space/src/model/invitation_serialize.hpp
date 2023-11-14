#pragma once

#include "space_invitation.hpp"

#include <userver/formats/json/value.hpp>
#include <userver/formats/parse/common_containers.hpp>
#include <userver/formats/serialize/common_containers.hpp>
#include <userver/utest/using_namespace_userver.hpp>

namespace svetit::space::model {

formats::json::Value Serialize(
	const SpaceInvitation& si,
	formats::serialize::To<formats::json::Value>);

SpaceInvitation Parse(const formats::json::Value& json,
	formats::parse::To<SpaceInvitation>);

} // namespace svetit::space::model