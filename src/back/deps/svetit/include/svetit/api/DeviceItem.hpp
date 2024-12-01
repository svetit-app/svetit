#pragma once

#include "DeviceItem_fwd.hpp"

#include <cstdint>
#include <optional>
#include <string>

#include <userver/chaotic/type_bundle_hpp.hpp>

namespace svetit {
namespace api {

struct DeviceItem {
  static constexpr auto kIdMinimum = 0;

  static constexpr auto kDeviceidMinimum = 1;

  static constexpr auto kTypeidMinimum = 1;

  std::optional<std::int64_t> id{};
  std::int64_t deviceId{};
  std::int64_t typeId{};
  std::string name{};
};

bool operator==(const svetit::api::DeviceItem& lhs,
                const svetit::api::DeviceItem& rhs);

USERVER_NAMESPACE::logging::LogHelper& operator<<(
    USERVER_NAMESPACE::logging::LogHelper& lh,
    const svetit::api::DeviceItem& value);

DeviceItem Parse(
    USERVER_NAMESPACE::formats::json::Value json,
    USERVER_NAMESPACE::formats::parse::To<svetit::api::DeviceItem>);

USERVER_NAMESPACE::formats::json::Value Serialize(
    const svetit::api::DeviceItem& value,
    USERVER_NAMESPACE::formats::serialize::To<
        USERVER_NAMESPACE::formats::json::Value>);

}  // namespace api
}  // namespace svetit

