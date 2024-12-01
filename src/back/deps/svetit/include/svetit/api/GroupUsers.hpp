#pragma once

#include "GroupUsers_fwd.hpp"

#include <cstdint>
#include <optional>
#include <string>
#include <userver/chaotic/io/std/vector.hpp>

#include <GroupUser.hpp>

#include <userver/chaotic/type_bundle_hpp.hpp>

namespace svetit {
namespace api {

struct GroupUsers {
  static constexpr auto kTotalMinimum = 0;

  std::vector<svetit::api::GroupUser> list{};
  std::int64_t total{};
};

bool operator==(const svetit::api::GroupUsers& lhs,
                const svetit::api::GroupUsers& rhs);

USERVER_NAMESPACE::logging::LogHelper& operator<<(
    USERVER_NAMESPACE::logging::LogHelper& lh,
    const svetit::api::GroupUsers& value);

GroupUsers Parse(
    USERVER_NAMESPACE::formats::json::Value json,
    USERVER_NAMESPACE::formats::parse::To<svetit::api::GroupUsers>);

USERVER_NAMESPACE::formats::json::Value Serialize(
    const svetit::api::GroupUsers& value,
    USERVER_NAMESPACE::formats::serialize::To<
        USERVER_NAMESPACE::formats::json::Value>);

}  // namespace api
}  // namespace svetit

