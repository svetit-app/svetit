#pragma once

#include "CcTypeParam_fwd.hpp"

#include <cstdint>
#include <optional>

#include <userver/chaotic/type_bundle_hpp.hpp>

namespace svetit {
namespace api {

struct CcTypeParam {
  static constexpr auto kCctypeidMinimum = 0;

  static constexpr auto kParamidMinimum = 0;

  std::int64_t ccTypeId{};
  std::int64_t paramId{};
};

bool operator==(const svetit::api::CcTypeParam& lhs,
                const svetit::api::CcTypeParam& rhs);

USERVER_NAMESPACE::logging::LogHelper& operator<<(
    USERVER_NAMESPACE::logging::LogHelper& lh,
    const svetit::api::CcTypeParam& value);

CcTypeParam Parse(
    USERVER_NAMESPACE::formats::json::Value json,
    USERVER_NAMESPACE::formats::parse::To<svetit::api::CcTypeParam>);

USERVER_NAMESPACE::formats::json::Value Serialize(
    const svetit::api::CcTypeParam& value,
    USERVER_NAMESPACE::formats::serialize::To<
        USERVER_NAMESPACE::formats::json::Value>);

}  // namespace api
}  // namespace svetit

