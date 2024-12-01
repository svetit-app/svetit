#pragma once

#include "CcStatusTypes_fwd.hpp"

#include <cstdint>
#include <optional>
#include <string>
#include <userver/chaotic/io/std/vector.hpp>

#include <CcStatusType.hpp>

#include <userver/chaotic/type_bundle_hpp.hpp>

namespace svetit {
namespace api {

struct CcStatusTypes {
  static constexpr auto kTotalMinimum = 0;

  std::vector<svetit::api::CcStatusType> list{};
  std::int64_t total{};
};

bool operator==(const svetit::api::CcStatusTypes& lhs,
                const svetit::api::CcStatusTypes& rhs);

USERVER_NAMESPACE::logging::LogHelper& operator<<(
    USERVER_NAMESPACE::logging::LogHelper& lh,
    const svetit::api::CcStatusTypes& value);

CcStatusTypes Parse(
    USERVER_NAMESPACE::formats::json::Value json,
    USERVER_NAMESPACE::formats::parse::To<svetit::api::CcStatusTypes>);

USERVER_NAMESPACE::formats::json::Value Serialize(
    const svetit::api::CcStatusTypes& value,
    USERVER_NAMESPACE::formats::serialize::To<
        USERVER_NAMESPACE::formats::json::Value>);

}  // namespace api
}  // namespace svetit

