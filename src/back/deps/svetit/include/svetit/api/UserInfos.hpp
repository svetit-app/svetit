#pragma once

#include "UserInfos_fwd.hpp"

#include <optional>
#include <string>
#include <userver/chaotic/io/boost/uuids/uuid.hpp>
#include <userver/chaotic/io/std/vector.hpp>

#include <UserInfo.hpp>

#include <userver/chaotic/type_bundle_hpp.hpp>

namespace svetit {
namespace api {

struct UserInfos {
  std::vector<svetit::api::UserInfo> list{};
};

bool operator==(const svetit::api::UserInfos& lhs,
                const svetit::api::UserInfos& rhs);

USERVER_NAMESPACE::logging::LogHelper& operator<<(
    USERVER_NAMESPACE::logging::LogHelper& lh,
    const svetit::api::UserInfos& value);

UserInfos Parse(USERVER_NAMESPACE::formats::json::Value json,
                USERVER_NAMESPACE::formats::parse::To<svetit::api::UserInfos>);

USERVER_NAMESPACE::formats::json::Value Serialize(
    const svetit::api::UserInfos& value,
    USERVER_NAMESPACE::formats::serialize::To<
        USERVER_NAMESPACE::formats::json::Value>);

}  // namespace api
}  // namespace svetit

