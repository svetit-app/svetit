#pragma once

#include "SectionParams_fwd.hpp"

#include <cstdint>
#include <optional>
#include <userver/chaotic/io/std/vector.hpp>

#include <SectionParam.hpp>

#include <userver/chaotic/type_bundle_hpp.hpp>

namespace svetit {
namespace api {

struct SectionParams {
  static constexpr auto kTotalMinimum = 0;

  std::vector<svetit::api::SectionParam> list{};
  std::int64_t total{};
};

bool operator==(const svetit::api::SectionParams& lhs,
                const svetit::api::SectionParams& rhs);

USERVER_NAMESPACE::logging::LogHelper& operator<<(
    USERVER_NAMESPACE::logging::LogHelper& lh,
    const svetit::api::SectionParams& value);

SectionParams Parse(
    USERVER_NAMESPACE::formats::json::Value json,
    USERVER_NAMESPACE::formats::parse::To<svetit::api::SectionParams>);

USERVER_NAMESPACE::formats::json::Value Serialize(
    const svetit::api::SectionParams& value,
    USERVER_NAMESPACE::formats::serialize::To<
        USERVER_NAMESPACE::formats::json::Value>);

}  // namespace api
}  // namespace svetit

