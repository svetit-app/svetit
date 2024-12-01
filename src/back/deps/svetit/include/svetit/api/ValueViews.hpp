#pragma once

#include "ValueViews_fwd.hpp"

#include <cstdint>
#include <optional>
#include <string>
#include <userver/chaotic/io/std/vector.hpp>

#include <ValueView.hpp>

#include <userver/chaotic/type_bundle_hpp.hpp>

namespace svetit {
namespace api {

struct ValueViews {
  static constexpr auto kTotalMinimum = 0;

  std::vector<svetit::api::ValueView> list{};
  std::int64_t total{};
};

bool operator==(const svetit::api::ValueViews& lhs,
                const svetit::api::ValueViews& rhs);

USERVER_NAMESPACE::logging::LogHelper& operator<<(
    USERVER_NAMESPACE::logging::LogHelper& lh,
    const svetit::api::ValueViews& value);

ValueViews Parse(
    USERVER_NAMESPACE::formats::json::Value json,
    USERVER_NAMESPACE::formats::parse::To<svetit::api::ValueViews>);

USERVER_NAMESPACE::formats::json::Value Serialize(
    const svetit::api::ValueViews& value,
    USERVER_NAMESPACE::formats::serialize::To<
        USERVER_NAMESPACE::formats::json::Value>);

}  // namespace api
}  // namespace svetit

