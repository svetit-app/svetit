#pragma once

#include "Invitations_fwd.hpp"

#include <cstdint>
#include <optional>
#include <string>
#include <userver/chaotic/io/boost/uuids/uuid.hpp>
#include <userver/chaotic/io/std/vector.hpp>

#include <Invitation.hpp>

#include <userver/chaotic/type_bundle_hpp.hpp>

namespace svetit {
namespace api {

struct Invitations {
  static constexpr auto kTotalMinimum = 0;

  std::vector<svetit::api::Invitation> list{};
  std::int64_t total{};
};

bool operator==(const svetit::api::Invitations& lhs,
                const svetit::api::Invitations& rhs);

USERVER_NAMESPACE::logging::LogHelper& operator<<(
    USERVER_NAMESPACE::logging::LogHelper& lh,
    const svetit::api::Invitations& value);

Invitations Parse(
    USERVER_NAMESPACE::formats::json::Value json,
    USERVER_NAMESPACE::formats::parse::To<svetit::api::Invitations>);

USERVER_NAMESPACE::formats::json::Value Serialize(
    const svetit::api::Invitations& value,
    USERVER_NAMESPACE::formats::serialize::To<
        USERVER_NAMESPACE::formats::json::Value>);

}  // namespace api
}  // namespace svetit

