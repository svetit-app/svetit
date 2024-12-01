#pragma once

#include "DiTypes_fwd.hpp"

#include <cstdint>
#include <optional>
#include <string>
#include <userver/chaotic/io/std/vector.hpp>

#include <DiType.hpp>

#include <userver/chaotic/type_bundle_hpp.hpp>

namespace svetit {
namespace api {

struct DiTypes {
  static constexpr auto kTotalMinimum = 0;

  std::vector<svetit::api::DiType> list{};
  std::int64_t total{};
};

bool operator==(const svetit::api::DiTypes& lhs,
                const svetit::api::DiTypes& rhs);

USERVER_NAMESPACE::logging::LogHelper& operator<<(
    USERVER_NAMESPACE::logging::LogHelper& lh,
    const svetit::api::DiTypes& value);

DiTypes Parse(USERVER_NAMESPACE::formats::json::Value json,
              USERVER_NAMESPACE::formats::parse::To<svetit::api::DiTypes>);

USERVER_NAMESPACE::formats::json::Value Serialize(
    const svetit::api::DiTypes& value,
    USERVER_NAMESPACE::formats::serialize::To<
        USERVER_NAMESPACE::formats::json::Value>);

}  // namespace api
}  // namespace svetit

