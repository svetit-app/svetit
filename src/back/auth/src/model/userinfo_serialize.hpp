#pragma once

#include "userinfo.hpp"

#include <userver/formats/json/value.hpp>
#include <userver/formats/parse/common_containers.hpp>
#include <userver/formats/serialize/common_containers.hpp>
#include <userver/utest/using_namespace_userver.hpp>

namespace svetit::auth::model {

formats::json::Value Serialize(
	const model::UserInfo& s,
	formats::serialize::To<formats::json::Value>);
model::UserInfo Parse(const formats::json::Value& json, formats::parse::To<UserInfo>);

// MapFromOIDCUserInfo - мапит json из keyclock в model::UserInfo
model::UserInfo MapFromOIDCUserInfo(const formats::json::Value& json);
model::UserInfo MapFromOIDCUserInfoById(const formats::json::Value& json);
std::vector<UserInfo> MapFromOIDCUserInfoList(const formats::json::Value& json);

} // namespace svetit::auth::model