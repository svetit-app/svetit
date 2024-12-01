#pragma once

#include "CcDis_fwd.hpp"

#include <cstdint>
#include <optional>
#include <userver/chaotic/io/std/vector.hpp>

#include <CcDi.hpp>

#include <userver/chaotic/type_bundle_hpp.hpp>

namespace svetit {
namespace api {

struct CcDis {
  static constexpr auto kTotalMinimum = 0;

  std::vector<svetit::api::CcDi> list{};
  std::int64_t total{};
};

bool operator==(const svetit::api::CcDis& lhs, const svetit::api::CcDis& rhs);

USERVER_NAMESPACE::logging::LogHelper& operator<<(
    USERVER_NAMESPACE::logging::LogHelper& lh, const svetit::api::CcDis& value);

CcDis Parse(USERVER_NAMESPACE::formats::json::Value json,
            USERVER_NAMESPACE::formats::parse::To<svetit::api::CcDis>);

USERVER_NAMESPACE::formats::json::Value Serialize(
    const svetit::api::CcDis& value,
    USERVER_NAMESPACE::formats::serialize::To<
        USERVER_NAMESPACE::formats::json::Value>);

}  // namespace api
}  // namespace svetit

