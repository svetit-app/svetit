#pragma once

#include "CcModeTypes_fwd.hpp"

#include <cstdint>
#include <optional>
#include <string>
#include <userver/chaotic/io/std/vector.hpp>

#include <CcModeType.hpp>

#include <userver/chaotic/type_bundle_hpp.hpp>

namespace svetit {
namespace api {

struct CcModeTypes {
  static constexpr auto kTotalMinimum = 0;

  std::vector<svetit::api::CcModeType> list{};
  std::int64_t total{};
};

bool operator==(const svetit::api::CcModeTypes& lhs,
                const svetit::api::CcModeTypes& rhs);

USERVER_NAMESPACE::logging::LogHelper& operator<<(
    USERVER_NAMESPACE::logging::LogHelper& lh,
    const svetit::api::CcModeTypes& value);

CcModeTypes Parse(
    USERVER_NAMESPACE::formats::json::Value json,
    USERVER_NAMESPACE::formats::parse::To<svetit::api::CcModeTypes>);

USERVER_NAMESPACE::formats::json::Value Serialize(
    const svetit::api::CcModeTypes& value,
    USERVER_NAMESPACE::formats::serialize::To<
        USERVER_NAMESPACE::formats::json::Value>);

}  // namespace api
}  // namespace svetit

