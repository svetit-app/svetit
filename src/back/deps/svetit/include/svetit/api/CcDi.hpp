#pragma once

#include "CcDi_fwd.hpp"

#include <cstdint>
#include <optional>

#include <userver/chaotic/type_bundle_hpp.hpp>

namespace svetit {
namespace api {

struct CcDi {
  static constexpr auto kCcidMinimum = 0;

  static constexpr auto kDiidMinimum = 0;

  std::int64_t ccId{};
  std::int64_t diId{};
};

bool operator==(const svetit::api::CcDi& lhs, const svetit::api::CcDi& rhs);

USERVER_NAMESPACE::logging::LogHelper& operator<<(
    USERVER_NAMESPACE::logging::LogHelper& lh, const svetit::api::CcDi& value);

CcDi Parse(USERVER_NAMESPACE::formats::json::Value json,
           USERVER_NAMESPACE::formats::parse::To<svetit::api::CcDi>);

USERVER_NAMESPACE::formats::json::Value Serialize(
    const svetit::api::CcDi& value,
    USERVER_NAMESPACE::formats::serialize::To<
        USERVER_NAMESPACE::formats::json::Value>);

}  // namespace api
}  // namespace svetit

