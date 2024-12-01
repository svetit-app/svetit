#pragma once

#include "SaveTimers_fwd.hpp"

#include <cstdint>
#include <optional>
#include <string>
#include <userver/chaotic/io/boost/uuids/uuid.hpp>
#include <userver/chaotic/io/std/vector.hpp>

#include <SaveTimer.hpp>

#include <userver/chaotic/type_bundle_hpp.hpp>

namespace svetit {
namespace api {

struct SaveTimers {
  static constexpr auto kTotalMinimum = 0;

  std::vector<svetit::api::SaveTimer> list{};
  std::int64_t total{};
};

bool operator==(const svetit::api::SaveTimers& lhs,
                const svetit::api::SaveTimers& rhs);

USERVER_NAMESPACE::logging::LogHelper& operator<<(
    USERVER_NAMESPACE::logging::LogHelper& lh,
    const svetit::api::SaveTimers& value);

SaveTimers Parse(
    USERVER_NAMESPACE::formats::json::Value json,
    USERVER_NAMESPACE::formats::parse::To<svetit::api::SaveTimers>);

USERVER_NAMESPACE::formats::json::Value Serialize(
    const svetit::api::SaveTimers& value,
    USERVER_NAMESPACE::formats::serialize::To<
        USERVER_NAMESPACE::formats::json::Value>);

}  // namespace api
}  // namespace svetit
