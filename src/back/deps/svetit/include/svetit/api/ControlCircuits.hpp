#pragma once

#include "ControlCircuits_fwd.hpp"

#include <cstdint>
#include <optional>
#include <string>
#include <userver/chaotic/io/std/vector.hpp>

#include <ControlCircuit.hpp>

#include <userver/chaotic/type_bundle_hpp.hpp>

namespace svetit {
namespace api {

struct ControlCircuits {
  static constexpr auto kTotalMinimum = 0;

  std::vector<svetit::api::ControlCircuit> list{};
  std::int64_t total{};
};

bool operator==(const svetit::api::ControlCircuits& lhs,
                const svetit::api::ControlCircuits& rhs);

USERVER_NAMESPACE::logging::LogHelper& operator<<(
    USERVER_NAMESPACE::logging::LogHelper& lh,
    const svetit::api::ControlCircuits& value);

ControlCircuits Parse(
    USERVER_NAMESPACE::formats::json::Value json,
    USERVER_NAMESPACE::formats::parse::To<svetit::api::ControlCircuits>);

USERVER_NAMESPACE::formats::json::Value Serialize(
    const svetit::api::ControlCircuits& value,
    USERVER_NAMESPACE::formats::serialize::To<
        USERVER_NAMESPACE::formats::json::Value>);

}  // namespace api
}  // namespace svetit

