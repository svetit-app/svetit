#pragma once

#include "Spaces_fwd.hpp"

#include <cstdint>
#include <optional>
#include <string>
#include <userver/chaotic/io/boost/uuids/uuid.hpp>
#include <userver/chaotic/io/std/vector.hpp>

#include <Space.hpp>

#include <userver/chaotic/type_bundle_hpp.hpp>

namespace svetit {
namespace api {

struct Spaces {
  static constexpr auto kTotalMinimum = 0;

  std::vector<svetit::api::Space> list{};
  std::int64_t total{};
};

bool operator==(const svetit::api::Spaces& lhs, const svetit::api::Spaces& rhs);

USERVER_NAMESPACE::logging::LogHelper& operator<<(
    USERVER_NAMESPACE::logging::LogHelper& lh,
    const svetit::api::Spaces& value);

Spaces Parse(USERVER_NAMESPACE::formats::json::Value json,
             USERVER_NAMESPACE::formats::parse::To<svetit::api::Spaces>);

USERVER_NAMESPACE::formats::json::Value Serialize(
    const svetit::api::Spaces& value,
    USERVER_NAMESPACE::formats::serialize::To<
        USERVER_NAMESPACE::formats::json::Value>);

}  // namespace api
}  // namespace svetit

