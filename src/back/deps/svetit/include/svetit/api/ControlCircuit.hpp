#pragma once

#include "ControlCircuit_fwd.hpp"

#include <cstdint>
#include <optional>
#include <string>

#include <userver/chaotic/type_bundle_hpp.hpp>

namespace svetit {
namespace api {

struct ControlCircuit {
  static constexpr auto kIdMinimum = 0;

  static constexpr auto kTypeidMinimum = 1;

  static constexpr auto kSectionidMinimum = 1;

  std::optional<std::int64_t> id{};
  std::int64_t typeId{};
  std::int64_t sectionId{};
  std::string name{};
};

bool operator==(const svetit::api::ControlCircuit& lhs,
                const svetit::api::ControlCircuit& rhs);

USERVER_NAMESPACE::logging::LogHelper& operator<<(
    USERVER_NAMESPACE::logging::LogHelper& lh,
    const svetit::api::ControlCircuit& value);

ControlCircuit Parse(
    USERVER_NAMESPACE::formats::json::Value json,
    USERVER_NAMESPACE::formats::parse::To<svetit::api::ControlCircuit>);

USERVER_NAMESPACE::formats::json::Value Serialize(
    const svetit::api::ControlCircuit& value,
    USERVER_NAMESPACE::formats::serialize::To<
        USERVER_NAMESPACE::formats::json::Value>);

}  // namespace api
}  // namespace svetit

