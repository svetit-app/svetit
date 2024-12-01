#pragma once

#include "User_fwd.hpp"

#include <cstdint>
#include <optional>
#include <string>
#include <userver/chaotic/io/boost/uuids/uuid.hpp>

#include <userver/chaotic/type_bundle_hpp.hpp>

namespace svetit {
namespace api {

struct User {
  // CppStringWithFormat

  static constexpr auto kRoleidMinimum = 1;

  std::string userId{};
  boost::uuids::uuid spaceId{};
  bool isOwner{};
  std::int64_t joinedAt{};
  std::int32_t roleId{};
};

bool operator==(const svetit::api::User& lhs, const svetit::api::User& rhs);

USERVER_NAMESPACE::logging::LogHelper& operator<<(
    USERVER_NAMESPACE::logging::LogHelper& lh, const svetit::api::User& value);

User Parse(USERVER_NAMESPACE::formats::json::Value json,
           USERVER_NAMESPACE::formats::parse::To<svetit::api::User>);

USERVER_NAMESPACE::formats::json::Value Serialize(
    const svetit::api::User& value,
    USERVER_NAMESPACE::formats::serialize::To<
        USERVER_NAMESPACE::formats::json::Value>);

}  // namespace api
}  // namespace svetit

