#pragma once

#include "CcStatusType_fwd.hpp"

#include <cstdint>
#include <optional>
#include <string>

#include <userver/chaotic/type_bundle_hpp.hpp>

namespace svetit {
namespace api {

struct CcStatusType {
  static constexpr auto kIdMinimum = 0;

  static constexpr auto kCctypeidMinimum = 1;

  static constexpr auto kCategoryidMinimum = 1;

  std::optional<std::int64_t> id{};
  std::int64_t ccTypeId{};
  std::int64_t categoryId{};
  std::string key{};
  std::string text{};
  bool inform{};
};

bool operator==(const svetit::api::CcStatusType& lhs,
                const svetit::api::CcStatusType& rhs);

USERVER_NAMESPACE::logging::LogHelper& operator<<(
    USERVER_NAMESPACE::logging::LogHelper& lh,
    const svetit::api::CcStatusType& value);

CcStatusType Parse(
    USERVER_NAMESPACE::formats::json::Value json,
    USERVER_NAMESPACE::formats::parse::To<svetit::api::CcStatusType>);

USERVER_NAMESPACE::formats::json::Value Serialize(
    const svetit::api::CcStatusType& value,
    USERVER_NAMESPACE::formats::serialize::To<
        USERVER_NAMESPACE::formats::json::Value>);

}  // namespace api
}  // namespace svetit

