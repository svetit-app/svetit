#pragma once

#include "DevicePluginParams_fwd.hpp"

#include <cstdint>
#include <optional>
#include <userver/chaotic/io/std/vector.hpp>

#include <DevicePluginParam.hpp>

#include <userver/chaotic/type_bundle_hpp.hpp>

namespace svetit {
namespace api {

struct DevicePluginParams {
  static constexpr auto kTotalMinimum = 0;

  std::vector<svetit::api::DevicePluginParam> list{};
  std::int64_t total{};
};

bool operator==(const svetit::api::DevicePluginParams& lhs,
                const svetit::api::DevicePluginParams& rhs);

USERVER_NAMESPACE::logging::LogHelper& operator<<(
    USERVER_NAMESPACE::logging::LogHelper& lh,
    const svetit::api::DevicePluginParams& value);

DevicePluginParams Parse(
    USERVER_NAMESPACE::formats::json::Value json,
    USERVER_NAMESPACE::formats::parse::To<svetit::api::DevicePluginParams>);

USERVER_NAMESPACE::formats::json::Value Serialize(
    const svetit::api::DevicePluginParams& value,
    USERVER_NAMESPACE::formats::serialize::To<
        USERVER_NAMESPACE::formats::json::Value>);

}  // namespace api
}  // namespace svetit

