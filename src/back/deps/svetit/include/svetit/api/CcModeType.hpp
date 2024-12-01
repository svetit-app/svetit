#pragma once

#include "CcModeType_fwd.hpp"

#include <cstdint>
#include <optional>
#include <string>

#include <userver/chaotic/type_bundle_hpp.hpp>

namespace svetit {
namespace api {

struct CcModeType {
  static constexpr auto kIdMinimum = 0;

  static constexpr auto kCctypeidMinimum = 1;

  std::optional<std::int64_t> id{};
  std::int64_t ccTypeId{};
  std::string key{};
  std::string name{};
};

bool operator==(const svetit::api::CcModeType& lhs,
                const svetit::api::CcModeType& rhs);

USERVER_NAMESPACE::logging::LogHelper& operator<<(
    USERVER_NAMESPACE::logging::LogHelper& lh,
    const svetit::api::CcModeType& value);

CcModeType Parse(
    USERVER_NAMESPACE::formats::json::Value json,
    USERVER_NAMESPACE::formats::parse::To<svetit::api::CcModeType>);

USERVER_NAMESPACE::formats::json::Value Serialize(
    const svetit::api::CcModeType& value,
    USERVER_NAMESPACE::formats::serialize::To<
        USERVER_NAMESPACE::formats::json::Value>);

}  // namespace api
}  // namespace svetit

