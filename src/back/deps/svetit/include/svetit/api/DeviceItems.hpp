#pragma once

#include "DeviceItems_fwd.hpp"

#include <cstdint>
#include <optional>
#include <string>
#include <userver/chaotic/io/std/vector.hpp>

#include <DeviceItem.hpp>

#include <userver/chaotic/type_bundle_hpp.hpp>

namespace svetit {
namespace api {

struct DeviceItems {
  static constexpr auto kTotalMinimum = 0;

  std::vector<svetit::api::DeviceItem> list{};
  std::int64_t total{};
};

bool operator==(const svetit::api::DeviceItems& lhs,
                const svetit::api::DeviceItems& rhs);

USERVER_NAMESPACE::logging::LogHelper& operator<<(
    USERVER_NAMESPACE::logging::LogHelper& lh,
    const svetit::api::DeviceItems& value);

DeviceItems Parse(
    USERVER_NAMESPACE::formats::json::Value json,
    USERVER_NAMESPACE::formats::parse::To<svetit::api::DeviceItems>);

USERVER_NAMESPACE::formats::json::Value Serialize(
    const svetit::api::DeviceItems& value,
    USERVER_NAMESPACE::formats::serialize::To<
        USERVER_NAMESPACE::formats::json::Value>);

}  // namespace api
}  // namespace svetit

