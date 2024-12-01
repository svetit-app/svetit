#pragma once

#include "DiPluginParams_fwd.hpp"

#include <cstdint>
#include <optional>
#include <userver/chaotic/io/std/vector.hpp>

#include <DiPluginParam.hpp>

#include <userver/chaotic/type_bundle_hpp.hpp>

namespace svetit {
namespace api {

struct DiPluginParams {
  static constexpr auto kTotalMinimum = 0;

  std::vector<svetit::api::DiPluginParam> list{};
  std::int64_t total{};
};

bool operator==(const svetit::api::DiPluginParams& lhs,
                const svetit::api::DiPluginParams& rhs);

USERVER_NAMESPACE::logging::LogHelper& operator<<(
    USERVER_NAMESPACE::logging::LogHelper& lh,
    const svetit::api::DiPluginParams& value);

DiPluginParams Parse(
    USERVER_NAMESPACE::formats::json::Value json,
    USERVER_NAMESPACE::formats::parse::To<svetit::api::DiPluginParams>);

USERVER_NAMESPACE::formats::json::Value Serialize(
    const svetit::api::DiPluginParams& value,
    USERVER_NAMESPACE::formats::serialize::To<
        USERVER_NAMESPACE::formats::json::Value>);

}  // namespace api
}  // namespace svetit

