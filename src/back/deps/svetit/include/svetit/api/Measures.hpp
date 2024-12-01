#pragma once

#include "Measures_fwd.hpp"

#include <cstdint>
#include <optional>
#include <string>
#include <userver/chaotic/io/boost/uuids/uuid.hpp>
#include <userver/chaotic/io/std/vector.hpp>

#include <Measure.hpp>

#include <userver/chaotic/type_bundle_hpp.hpp>

namespace svetit {
namespace api {

struct Measures {
  static constexpr auto kTotalMinimum = 0;

  std::vector<svetit::api::Measure> list{};
  std::int64_t total{};
};

bool operator==(const svetit::api::Measures& lhs,
                const svetit::api::Measures& rhs);

USERVER_NAMESPACE::logging::LogHelper& operator<<(
    USERVER_NAMESPACE::logging::LogHelper& lh,
    const svetit::api::Measures& value);

Measures Parse(USERVER_NAMESPACE::formats::json::Value json,
               USERVER_NAMESPACE::formats::parse::To<svetit::api::Measures>);

USERVER_NAMESPACE::formats::json::Value Serialize(
    const svetit::api::Measures& value,
    USERVER_NAMESPACE::formats::serialize::To<
        USERVER_NAMESPACE::formats::json::Value>);

}  // namespace api
}  // namespace svetit

