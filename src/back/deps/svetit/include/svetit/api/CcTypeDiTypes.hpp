#pragma once

#include "CcTypeDiTypes_fwd.hpp"

#include <cstdint>
#include <optional>
#include <userver/chaotic/io/std/vector.hpp>

#include <CcTypeDiType.hpp>

#include <userver/chaotic/type_bundle_hpp.hpp>

namespace svetit {
namespace api {

struct CcTypeDiTypes {
  static constexpr auto kTotalMinimum = 0;

  std::vector<svetit::api::CcTypeDiType> list{};
  std::int64_t total{};
};

bool operator==(const svetit::api::CcTypeDiTypes& lhs,
                const svetit::api::CcTypeDiTypes& rhs);

USERVER_NAMESPACE::logging::LogHelper& operator<<(
    USERVER_NAMESPACE::logging::LogHelper& lh,
    const svetit::api::CcTypeDiTypes& value);

CcTypeDiTypes Parse(
    USERVER_NAMESPACE::formats::json::Value json,
    USERVER_NAMESPACE::formats::parse::To<svetit::api::CcTypeDiTypes>);

USERVER_NAMESPACE::formats::json::Value Serialize(
    const svetit::api::CcTypeDiTypes& value,
    USERVER_NAMESPACE::formats::serialize::To<
        USERVER_NAMESPACE::formats::json::Value>);

}  // namespace api
}  // namespace svetit

