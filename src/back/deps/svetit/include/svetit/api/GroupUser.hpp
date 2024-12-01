#pragma once

#include "GroupUser_fwd.hpp"

#include <cstdint>
#include <optional>
#include <string>

#include <userver/chaotic/type_bundle_hpp.hpp>

namespace svetit {
namespace api {

struct GroupUser {
  static constexpr auto kNodeidMinimum = 0;

  std::int32_t nodeId{};
  std::string userId{};
};

bool operator==(const svetit::api::GroupUser& lhs,
                const svetit::api::GroupUser& rhs);

USERVER_NAMESPACE::logging::LogHelper& operator<<(
    USERVER_NAMESPACE::logging::LogHelper& lh,
    const svetit::api::GroupUser& value);

GroupUser Parse(USERVER_NAMESPACE::formats::json::Value json,
                USERVER_NAMESPACE::formats::parse::To<svetit::api::GroupUser>);

USERVER_NAMESPACE::formats::json::Value Serialize(
    const svetit::api::GroupUser& value,
    USERVER_NAMESPACE::formats::serialize::To<
        USERVER_NAMESPACE::formats::json::Value>);

}  // namespace api
}  // namespace svetit

