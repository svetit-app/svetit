#pragma once

#include "CcParams_fwd.hpp"

#include <cstdint>
#include <optional>
#include <userver/chaotic/io/std/vector.hpp>

#include <CcParam.hpp>

#include <userver/chaotic/type_bundle_hpp.hpp>

namespace svetit {
namespace api {

struct CcParams {
  static constexpr auto kTotalMinimum = 0;

  std::vector<svetit::api::CcParam> list{};
  std::int64_t total{};
};

bool operator==(const svetit::api::CcParams& lhs,
                const svetit::api::CcParams& rhs);

USERVER_NAMESPACE::logging::LogHelper& operator<<(
    USERVER_NAMESPACE::logging::LogHelper& lh,
    const svetit::api::CcParams& value);

CcParams Parse(USERVER_NAMESPACE::formats::json::Value json,
               USERVER_NAMESPACE::formats::parse::To<svetit::api::CcParams>);

USERVER_NAMESPACE::formats::json::Value Serialize(
    const svetit::api::CcParams& value,
    USERVER_NAMESPACE::formats::serialize::To<
        USERVER_NAMESPACE::formats::json::Value>);

}  // namespace api
}  // namespace svetit

