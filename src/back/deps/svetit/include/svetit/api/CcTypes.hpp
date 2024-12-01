#pragma once

#include "CcTypes_fwd.hpp"

#include <cstdint>
#include <optional>
#include <string>
#include <userver/chaotic/io/boost/uuids/uuid.hpp>
#include <userver/chaotic/io/std/vector.hpp>

#include <CcType.hpp>

#include <userver/chaotic/type_bundle_hpp.hpp>

namespace svetit {
namespace api {

struct CcTypes {
  static constexpr auto kTotalMinimum = 0;

  std::vector<svetit::api::CcType> list{};
  std::int64_t total{};
};

bool operator==(const svetit::api::CcTypes& lhs,
                const svetit::api::CcTypes& rhs);

USERVER_NAMESPACE::logging::LogHelper& operator<<(
    USERVER_NAMESPACE::logging::LogHelper& lh,
    const svetit::api::CcTypes& value);

CcTypes Parse(USERVER_NAMESPACE::formats::json::Value json,
              USERVER_NAMESPACE::formats::parse::To<svetit::api::CcTypes>);

USERVER_NAMESPACE::formats::json::Value Serialize(
    const svetit::api::CcTypes& value,
    USERVER_NAMESPACE::formats::serialize::To<
        USERVER_NAMESPACE::formats::json::Value>);

}  // namespace api
}  // namespace svetit

