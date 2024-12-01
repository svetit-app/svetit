#pragma once

#include "DiPluginParam_fwd.hpp"

#include <cstdint>
#include <optional>

#include <userver/chaotic/type_bundle_hpp.hpp>

namespace svetit {
namespace api {

struct DiPluginParam {
  static constexpr auto kDitypeidMinimum = 0;

  static constexpr auto kParamidMinimum = 0;

  std::int64_t diTypeId{};
  std::int64_t paramId{};
};

bool operator==(const svetit::api::DiPluginParam& lhs,
                const svetit::api::DiPluginParam& rhs);

USERVER_NAMESPACE::logging::LogHelper& operator<<(
    USERVER_NAMESPACE::logging::LogHelper& lh,
    const svetit::api::DiPluginParam& value);

DiPluginParam Parse(
    USERVER_NAMESPACE::formats::json::Value json,
    USERVER_NAMESPACE::formats::parse::To<svetit::api::DiPluginParam>);

USERVER_NAMESPACE::formats::json::Value Serialize(
    const svetit::api::DiPluginParam& value,
    USERVER_NAMESPACE::formats::serialize::To<
        USERVER_NAMESPACE::formats::json::Value>);

}  // namespace api
}  // namespace svetit

