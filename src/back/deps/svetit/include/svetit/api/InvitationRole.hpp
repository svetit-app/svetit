#pragma once

#include "InvitationRole_fwd.hpp"

#include <cstdint>
#include <optional>

#include <userver/chaotic/type_bundle_hpp.hpp>

namespace svetit {
namespace api {

struct InvitationRole {
  static constexpr auto kRoleidMinimum = 1;

  std::int32_t roleId{};
};

bool operator==(const svetit::api::InvitationRole& lhs,
                const svetit::api::InvitationRole& rhs);

USERVER_NAMESPACE::logging::LogHelper& operator<<(
    USERVER_NAMESPACE::logging::LogHelper& lh,
    const svetit::api::InvitationRole& value);

InvitationRole Parse(
    USERVER_NAMESPACE::formats::json::Value json,
    USERVER_NAMESPACE::formats::parse::To<svetit::api::InvitationRole>);

USERVER_NAMESPACE::formats::json::Value Serialize(
    const svetit::api::InvitationRole& value,
    USERVER_NAMESPACE::formats::serialize::To<
        USERVER_NAMESPACE::formats::json::Value>);

}  // namespace api
}  // namespace svetit

