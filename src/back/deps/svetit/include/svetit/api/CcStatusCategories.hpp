#pragma once

#include "CcStatusCategories_fwd.hpp"

#include <cstdint>
#include <optional>
#include <string>
#include <userver/chaotic/io/boost/uuids/uuid.hpp>
#include <userver/chaotic/io/std/vector.hpp>

#include <CcStatusCategory.hpp>

#include <userver/chaotic/type_bundle_hpp.hpp>

namespace svetit {
namespace api {

struct CcStatusCategories {
  static constexpr auto kTotalMinimum = 0;

  std::vector<svetit::api::CcStatusCategory> list{};
  std::int64_t total{};
};

bool operator==(const svetit::api::CcStatusCategories& lhs,
                const svetit::api::CcStatusCategories& rhs);

USERVER_NAMESPACE::logging::LogHelper& operator<<(
    USERVER_NAMESPACE::logging::LogHelper& lh,
    const svetit::api::CcStatusCategories& value);

CcStatusCategories Parse(
    USERVER_NAMESPACE::formats::json::Value json,
    USERVER_NAMESPACE::formats::parse::To<svetit::api::CcStatusCategories>);

USERVER_NAMESPACE::formats::json::Value Serialize(
    const svetit::api::CcStatusCategories& value,
    USERVER_NAMESPACE::formats::serialize::To<
        USERVER_NAMESPACE::formats::json::Value>);

}  // namespace api
}  // namespace svetit

