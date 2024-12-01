#pragma once

#include "Devices_fwd.hpp"

#include <cstdint>
#include <optional>
#include <string>
#include <userver/chaotic/io/boost/uuids/uuid.hpp>
#include <userver/chaotic/io/std/vector.hpp>

#include <Device.hpp>

#include <userver/chaotic/type_bundle_hpp.hpp>

namespace svetit {
namespace api {

struct Devices {
  static constexpr auto kTotalMinimum = 0;

  std::vector<svetit::api::Device> list{};
  std::int64_t total{};
};

bool operator==(const svetit::api::Devices& lhs,
                const svetit::api::Devices& rhs);

USERVER_NAMESPACE::logging::LogHelper& operator<<(
    USERVER_NAMESPACE::logging::LogHelper& lh,
    const svetit::api::Devices& value);

Devices Parse(USERVER_NAMESPACE::formats::json::Value json,
              USERVER_NAMESPACE::formats::parse::To<svetit::api::Devices>);

USERVER_NAMESPACE::formats::json::Value Serialize(
    const svetit::api::Devices& value,
    USERVER_NAMESPACE::formats::serialize::To<
        USERVER_NAMESPACE::formats::json::Value>);

}  // namespace api
}  // namespace svetit

