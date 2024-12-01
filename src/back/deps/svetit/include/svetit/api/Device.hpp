#pragma once

#include "Device_fwd.hpp"

#include <cstdint>
#include <optional>
#include <string>
#include <userver/chaotic/io/boost/uuids/uuid.hpp>

#include <userver/chaotic/type_bundle_hpp.hpp>

namespace svetit {
namespace api {

struct Device {
  static constexpr auto kIdMinimum = 0;

  // CppStringWithFormat

  static constexpr auto kPluginidMinimum = 1;

  std::optional<std::int64_t> id{};
  boost::uuids::uuid projectId{};
  std::int64_t pluginId{};
  std::string name{};
  std::int32_t checkIntervalMsec{};
};

bool operator==(const svetit::api::Device& lhs, const svetit::api::Device& rhs);

USERVER_NAMESPACE::logging::LogHelper& operator<<(
    USERVER_NAMESPACE::logging::LogHelper& lh,
    const svetit::api::Device& value);

Device Parse(USERVER_NAMESPACE::formats::json::Value json,
             USERVER_NAMESPACE::formats::parse::To<svetit::api::Device>);

USERVER_NAMESPACE::formats::json::Value Serialize(
    const svetit::api::Device& value,
    USERVER_NAMESPACE::formats::serialize::To<
        USERVER_NAMESPACE::formats::json::Value>);

}  // namespace api
}  // namespace svetit

