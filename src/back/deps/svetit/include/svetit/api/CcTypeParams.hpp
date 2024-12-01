#pragma once

#include "CcTypeParams_fwd.hpp"

#include <cstdint>
#include <optional>
#include <userver/chaotic/io/std/vector.hpp>

#include <CcTypeParam.hpp>

#include <userver/chaotic/type_bundle_hpp.hpp>

namespace svetit {
namespace api {

struct CcTypeParams {
  static constexpr auto kTotalMinimum = 0;

  std::vector<svetit::api::CcTypeParam> list{};
  std::int64_t total{};
};

bool operator==(const svetit::api::CcTypeParams& lhs,
                const svetit::api::CcTypeParams& rhs);

USERVER_NAMESPACE::logging::LogHelper& operator<<(
    USERVER_NAMESPACE::logging::LogHelper& lh,
    const svetit::api::CcTypeParams& value);

CcTypeParams Parse(
    USERVER_NAMESPACE::formats::json::Value json,
    USERVER_NAMESPACE::formats::parse::To<svetit::api::CcTypeParams>);

USERVER_NAMESPACE::formats::json::Value Serialize(
    const svetit::api::CcTypeParams& value,
    USERVER_NAMESPACE::formats::serialize::To<
        USERVER_NAMESPACE::formats::json::Value>);

}  // namespace api
}  // namespace svetit

