#pragma once

#include "Invitation_fwd.hpp"

#include <cstdint>
#include <optional>
#include <string>
#include <userver/chaotic/io/boost/uuids/uuid.hpp>

#include <userver/chaotic/type_bundle_hpp.hpp>

namespace svetit {
namespace api {

struct Invitation {
  static constexpr auto kIdMinimum = 0;

  // CppStringWithFormat

  static constexpr auto kRoleidMinimum = 1;

  std::optional<std::int32_t> id{};
  boost::uuids::uuid spaceId{};
  std::optional<std::string> creatorId{};
  std::string userId{};
  std::optional<std::int32_t> roleId{};
  std::optional<std::int64_t> createdAt{};
};

bool operator==(const svetit::api::Invitation& lhs,
                const svetit::api::Invitation& rhs);

USERVER_NAMESPACE::logging::LogHelper& operator<<(
    USERVER_NAMESPACE::logging::LogHelper& lh,
    const svetit::api::Invitation& value);

Invitation Parse(
    USERVER_NAMESPACE::formats::json::Value json,
    USERVER_NAMESPACE::formats::parse::To<svetit::api::Invitation>);

USERVER_NAMESPACE::formats::json::Value Serialize(
    const svetit::api::Invitation& value,
    USERVER_NAMESPACE::formats::serialize::To<
        USERVER_NAMESPACE::formats::json::Value>);

}  // namespace api
}  // namespace svetit

