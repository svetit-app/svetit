#pragma once

#include "SpaceParams_fwd.hpp"

#include <cstdint>
#include <optional>

#include <userver/chaotic/type_bundle_hpp.hpp>

namespace svetit {
namespace api {

struct SpaceParams {
  static constexpr auto kInvitationsizeMinimum = 0;

  bool canCreate{};
  std::int32_t invitationSize{};
};

bool operator==(const svetit::api::SpaceParams& lhs,
                const svetit::api::SpaceParams& rhs);

USERVER_NAMESPACE::logging::LogHelper& operator<<(
    USERVER_NAMESPACE::logging::LogHelper& lh,
    const svetit::api::SpaceParams& value);

SpaceParams Parse(
    USERVER_NAMESPACE::formats::json::Value json,
    USERVER_NAMESPACE::formats::parse::To<svetit::api::SpaceParams>);

USERVER_NAMESPACE::formats::json::Value Serialize(
    const svetit::api::SpaceParams& value,
    USERVER_NAMESPACE::formats::serialize::To<
        USERVER_NAMESPACE::formats::json::Value>);

}  // namespace api
}  // namespace svetit

