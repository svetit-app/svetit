#pragma once

#include "DevicePluginParam_fwd.hpp"

#include <cstdint>
#include <optional>

#include <userver/chaotic/type_bundle_hpp.hpp>

namespace svetit {
namespace api {

struct DevicePluginParam {
  static constexpr auto kDeviceidMinimum = 0;

  static constexpr auto kParamidMinimum = 0;

  std::int64_t deviceId{};
  std::int64_t paramId{};
};

bool operator==(const svetit::api::DevicePluginParam& lhs,
                const svetit::api::DevicePluginParam& rhs);

USERVER_NAMESPACE::logging::LogHelper& operator<<(
    USERVER_NAMESPACE::logging::LogHelper& lh,
    const svetit::api::DevicePluginParam& value);

DevicePluginParam Parse(
    USERVER_NAMESPACE::formats::json::Value json,
    USERVER_NAMESPACE::formats::parse::To<svetit::api::DevicePluginParam>);

USERVER_NAMESPACE::formats::json::Value Serialize(
    const svetit::api::DevicePluginParam& value,
    USERVER_NAMESPACE::formats::serialize::To<
        USERVER_NAMESPACE::formats::json::Value>);

}  // namespace api
}  // namespace svetit

