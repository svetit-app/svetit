#pragma once

#include "CcTypeDiType_fwd.hpp"

#include <cstdint>
#include <optional>

#include <userver/chaotic/type_bundle_hpp.hpp>

namespace svetit {
namespace api {

struct CcTypeDiType {
  static constexpr auto kCctypeidMinimum = 0;

  static constexpr auto kDitypeidMinimum = 0;

  std::int64_t ccTypeId{};
  std::int64_t diTypeId{};
};

bool operator==(const svetit::api::CcTypeDiType& lhs,
                const svetit::api::CcTypeDiType& rhs);

USERVER_NAMESPACE::logging::LogHelper& operator<<(
    USERVER_NAMESPACE::logging::LogHelper& lh,
    const svetit::api::CcTypeDiType& value);

CcTypeDiType Parse(
    USERVER_NAMESPACE::formats::json::Value json,
    USERVER_NAMESPACE::formats::parse::To<svetit::api::CcTypeDiType>);

USERVER_NAMESPACE::formats::json::Value Serialize(
    const svetit::api::CcTypeDiType& value,
    USERVER_NAMESPACE::formats::serialize::To<
        USERVER_NAMESPACE::formats::json::Value>);

}  // namespace api
}  // namespace svetit

