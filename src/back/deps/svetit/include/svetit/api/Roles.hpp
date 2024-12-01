#pragma once

#include "Roles_fwd.hpp"

#include <cstdint>
#include <optional>
#include <string>
#include <userver/chaotic/io/std/vector.hpp>

#include <Role.hpp>

#include <userver/chaotic/type_bundle_hpp.hpp>

namespace svetit {
namespace api {

struct Roles {
  static constexpr auto kTotalMinimum = 0;

  std::vector<svetit::api::Role> list{};
  std::int64_t total{};
};

bool operator==(const svetit::api::Roles& lhs, const svetit::api::Roles& rhs);

USERVER_NAMESPACE::logging::LogHelper& operator<<(
    USERVER_NAMESPACE::logging::LogHelper& lh, const svetit::api::Roles& value);

Roles Parse(USERVER_NAMESPACE::formats::json::Value json,
            USERVER_NAMESPACE::formats::parse::To<svetit::api::Roles>);

USERVER_NAMESPACE::formats::json::Value Serialize(
    const svetit::api::Roles& value,
    USERVER_NAMESPACE::formats::serialize::To<
        USERVER_NAMESPACE::formats::json::Value>);

}  // namespace api
}  // namespace svetit

