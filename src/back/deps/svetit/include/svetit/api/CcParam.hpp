#pragma once

#include "CcParam_fwd.hpp"

#include <cstdint>
#include <optional>

#include <userver/chaotic/type_bundle_hpp.hpp>

namespace svetit {
namespace api {

struct CcParam {
  static constexpr auto kCcidMinimum = 0;

  static constexpr auto kParamidMinimum = 0;

  std::int64_t ccId{};
  std::int64_t paramId{};
};

bool operator==(const svetit::api::CcParam& lhs,
                const svetit::api::CcParam& rhs);

USERVER_NAMESPACE::logging::LogHelper& operator<<(
    USERVER_NAMESPACE::logging::LogHelper& lh,
    const svetit::api::CcParam& value);

CcParam Parse(USERVER_NAMESPACE::formats::json::Value json,
              USERVER_NAMESPACE::formats::parse::To<svetit::api::CcParam>);

USERVER_NAMESPACE::formats::json::Value Serialize(
    const svetit::api::CcParam& value,
    USERVER_NAMESPACE::formats::serialize::To<
        USERVER_NAMESPACE::formats::json::Value>);

}  // namespace api
}  // namespace svetit

